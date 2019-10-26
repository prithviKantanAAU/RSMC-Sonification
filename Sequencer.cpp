#include "Sequencer.h"

Sequencer::Sequencer()
{
	initialize();
}

Sequencer::~Sequencer()
{}

void Sequencer::initialize()
{
	/*for (int i = 0; i < numActiveFiles; i++)
	{
		readMusicCSV(i, customPath, customMusic);
	}
	
	bool testFlag = false;

	if (testFlag)
	{
		for (int i = 0; i < 20; i++)
		{
			readMusicCSV(i, defaultPath, defaultMusic);
		}
	}*/

	/*if (defaultMode)
		currentMusic = defaultMusic[musicIndex_default];
	else
		currentMusic = customMusic[musicIndex_custom];*/

	resetCounters();
	songStructureIndex = fetchNewRandomIndex(musicStructure.numSongStructures);
}

void Sequencer::resetCounters()
{
	pulsesElapsed = 0;
	beatsElapsed = 0;
	barsElapsed = 0;
	currentMelodicMeasure = 1;
	beatsElapsed_withinBar = 0;
	barsElapsed_withinMeasure = 0;
	measuresElapsed_withinMovement = 0;
	sixteenthCounter_withinMeasure = 0;
	sixteenthCounter_withinBar = 0;
}

void Sequencer::incrementPulseCounter()
{
	pulsesElapsed++;

	//Increment Global Counters
	if ((pulsesElapsed - 1) % 4 == 0 && pulsesElapsed > 4)
	beatsElapsed++;
	if ((pulsesElapsed - 1) % 16 == 0 && pulsesElapsed > 16)
	barsElapsed++;
	if ((pulsesElapsed - 1) % 64 == 0 && pulsesElapsed > 64)
	currentMelodicMeasure++;

	//Increment Positional Counters
	beatsElapsed_withinBar = beatsElapsed % beatsInBar;
	barsElapsed_withinMeasure = barsElapsed % barsInMelMeasure;
	lastBarCondition = (barsElapsed_withinMeasure == 3);

	//Update Local Musical Time Counters
	sixteenthCounter_withinMeasure = (pulsesElapsed - 1) % 64;
	sixteenthCounter_withinBar = (pulsesElapsed - 1) % 16;

	checkMeasureEnd();
	updateCurrentSelection();
}

void Sequencer::checkMeasureEnd()
{
	//Update Booleans
	barCompleted = ((pulsesElapsed - 1) % 16 == 0);
	melMeasureCompleted = ((pulsesElapsed - 1) % 64 == 0);
	percMeasureCompleted = ((pulsesElapsed - 1) % 128 == 0);
	fileCompleted = (currentMelodicMeasure == 25);
	
	currentMelodicMeasure = currentMelodicMeasure > 24 ? 1 : currentMelodicMeasure; // RESET
}

void Sequencer::updateCurrentSelection()
{
	short perc_BaseIndex = 0;
	if (melMeasureCompleted || percMeasureCompleted)
	 perc_BaseIndex = fetchPercBaseIndex();

	if (fileCompleted)
	{
		songStructureIndex = fetchNewRandomIndex(musicStructure.numSongStructures);
		openNextFile();
		fileCompleted = false;
	}

	if (percMeasureCompleted)
	{
		kickIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
		snareIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
		hhIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
		crashIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
		percMeasureCompleted = false;
	}

	if (melMeasureCompleted)
	{
		snareFillIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
		riffVelIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
		riffOctIndex = fetchNewRandomIndex(percPatterns.riffOctPatterns);
		arpDegIndex = fetchNewRandomIndex(percPatterns.numArpDegreePatterns);
		arpVelIndex = fetchNewRandomIndex(percPatterns.numArpVelPatterns);
		stabsIndex = fetchNewRandomIndex(percPatterns.numChordStabVelPatterns);
		currentScale = currentMusic.mScales[currentMusic.order[currentMelodicMeasure - 1] - 1];
		melMeasureCompleted = false;
	}
}

short Sequencer::fetchPercBaseIndex()
{
	short currentMuteConfiguration = musicStructure.songPatterns[songStructureIndex][currentMelodicMeasure - 1];
	short multiplier = 0;

	bool isL1 = std::find(std::begin(musicStructure.muteStructuresPerc_L1),
		std::end(musicStructure.muteStructuresPerc_L1), currentMuteConfiguration)
		!= std::end(musicStructure.muteStructuresPerc_L1);
	if (isL1)
		multiplier = 0;

	bool isL2 = std::find(std::begin(musicStructure.muteStructuresPerc_L2),
		std::end(musicStructure.muteStructuresPerc_L2), currentMuteConfiguration)
		!= std::end(musicStructure.muteStructuresPerc_L2);
	if (isL2)
		multiplier = 1;

	bool isL3 = (currentMuteConfiguration == musicStructure.muteStructuresPerc_L3);
	if (isL3)
		multiplier = 2;
	bool isL4 = (currentMuteConfiguration == musicStructure.muteStructuresPerc_L4);
	if (isL4)
		multiplier = 3;

	return multiplier * percPatterns.percPatternsPerLevel;
}

void Sequencer::randomizeRhythms()
{
	int perc_BaseIndex = fetchPercBaseIndex();
	kickIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
	snareIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
	hhIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
	riffVelIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
	riffOctIndex = fetchNewRandomIndex(percPatterns.riffOctPatterns);
	arpDegIndex = fetchNewRandomIndex(percPatterns.numArpDegreePatterns);
	arpVelIndex = fetchNewRandomIndex(percPatterns.numArpVelPatterns);
	stabsIndex = fetchNewRandomIndex(percPatterns.numChordStabVelPatterns);
	crashIndex = perc_BaseIndex + fetchNewRandomIndex(percPatterns.percPatternsPerLevel);
}

// Track Index // 1 : Kick // 2 : Snare // 3 : HH // 4 : Chords // 5 : Riff // 6 : Melody
// Track Type  // 1 : Perc // 2 : Mel   // 3 : Pad
// Info Type   // 1 : Vel  // 2 : Deg   // 3 : Oct/ChordType	
short Sequencer::fetchNewMusicInfo(int trackIndex, int infoType, int style)
{
	short temp = 0;
	short out = 0;
	
	short currentMotif = currentMusic.order[currentMelodicMeasure - 1] - 1;

	switch (trackIndex)
	{
	case 1:
		switch (style)
		{
		case 0:
			out = percPatterns.kick_S1[kickIndex][sixteenthCounter_withinBar];
			break;
		case 1:
			out = percPatterns.kick_S2[kickIndex][sixteenthCounter_withinBar];
			break;
		case 2:
			out = percPatterns.kick_S3[kickIndex][sixteenthCounter_withinBar];
			break;
		case 3:
			out = percPatterns.kick_S4[kickIndex][sixteenthCounter_withinBar];
			break;
		}
		break;
	case 2:
		switch (style)
		{
		case 0:
			if (lastBarCondition)
				out = percPatterns.snare_S1_Fills[snareFillIndex][sixteenthCounter_withinBar];
			else
				out = percPatterns.snare_S1[snareIndex][sixteenthCounter_withinBar];
			break;
		case 1:
			out = percPatterns.snare_S2[snareIndex][sixteenthCounter_withinBar];
			break;
		case 2:
			out = percPatterns.snare_S3[snareIndex][sixteenthCounter_withinBar];
			break;
		case 3:
			out = percPatterns.snare_S4[snareIndex][sixteenthCounter_withinBar];
			break;
		}
		break;
	case 3:
		switch (style)
		{
		case 0:
			out = percPatterns.hh_S1[hhIndex][sixteenthCounter_withinBar];
			break;
		case 1:
			out = percPatterns.hh_S2[hhIndex][sixteenthCounter_withinBar];
			break;
		case 2:
			out = percPatterns.hh_S3[hhIndex][sixteenthCounter_withinBar];
			break;
		case 3:
			out = percPatterns.hh_S4[hhIndex][sixteenthCounter_withinBar];
			break;
		}
		break;
	case 4:
		switch (infoType)
		{
		case 1:
			out = currentMusic.cv_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		case 2:
			out = currentMusic.cf_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		case 3:
			out = currentMusic.ct_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		}
		break;
	case 5:
		switch (infoType)
		{
		case 1:
			switch (style)
			{
			case 0:
				out = percPatterns.riffVel_S1[riffVelIndex][sixteenthCounter_withinBar];
				break;
			case 1:
				out = percPatterns.riffVel_S2[riffVelIndex][sixteenthCounter_withinBar];
				break;
			case 2:
				out = percPatterns.riffVel_S3[riffVelIndex][sixteenthCounter_withinBar];
				break;
			case 3:
				out = percPatterns.riffVel_S4[riffVelIndex][sixteenthCounter_withinBar];
				break;
			}
			break;
		case 2:
			out = currentMusic.cf_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		case 3:
			out = percPatterns.riffOct[riffOctIndex][sixteenthCounter_withinBar];
			break;
		}
		break;
	case 6:
		switch (infoType)
		{
		case 1:
			out = currentMusic.mv_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		case 2:
			out = currentMusic.mf_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		case 3:
			out = currentMusic.mo_Array[currentMotif][sixteenthCounter_withinMeasure];
			break;
		}
		break;
	case 7:
		switch (infoType)
		{
		case 1:
			if (musicStructure.arpMode[currentMelodicMeasure - 1] == 0)
			{
				switch (style)
				{
				case 0:
					out = percPatterns.chordStabsVel_S1[stabsIndex][sixteenthCounter_withinBar];
					break;
				case 1:
					out = percPatterns.chordStabsVel_S2[stabsIndex][sixteenthCounter_withinBar];
					break;
				case 2:
					out = percPatterns.chordStabsVel_S3[stabsIndex][sixteenthCounter_withinBar];
					break;
				case 3:
					out = percPatterns.chordStabsVel_S4[stabsIndex][sixteenthCounter_withinBar];
					break;
				}
			}
			else
			{
				out = percPatterns.arpVelocities[arpVelIndex][sixteenthCounter_withinBar];
			}
			break;
		case 2:
			out = percPatterns.arpDegrees[arpDegIndex][sixteenthCounter_withinBar];
			break;
		case 3:
			out = musicStructure.arpMode[currentMelodicMeasure - 1];
			break;
		}
		break;
	case 8:
		out = sixteenthCounter_withinMeasure == 0 ? 9 : 0;
		break;
	}
	return out;
}

int Sequencer::fetchNewRandomIndex(int range)
{
	return randGen.nextInt(range - 1);
}