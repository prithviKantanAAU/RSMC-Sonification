#pragma once

#define beatsInBar 4
#define barsInMelMeasure 4
#define melMeasuresInMovement 8
#define barsInPercMeasure 1

#include "../JuceLibraryCode/JuceHeader.h"
#include "MusicInfoRead.h"
#include "PercPatternLibrary.h"
#include "MusicStructure.h"

class Sequencer
{
public:

	Sequencer();
	~Sequencer();
	void initialize();
	void resetCounters();
	PercPatternLibrary percPatterns;
	MusicStructure musicStructure;
	Random randGen;
	bool isFileLoaded = false;
	int currentScale = 1;

	void checkMeasureEnd();
	void updateCurrentSelection();

	short fetchPercBaseIndex();
	int fetchNewRandomIndex(int range);

	MusicInfoRead currentMusic;

	//Default Music
	bool defaultMode = false;
	int numDefaultFiles = 20;
	MusicInfoRead defaultMusic[20];

	//Custom Music

	std::string customPath = "D:\\GaitSonification\\MusicCSVs\\";
	std::string defaultPath = "D:\\GaitSonification\\MusicCSVs\\Default ";
	MusicInfoRead customMusic[10];
	int numActiveFiles = 1;

	void loadNewFile(String name)
	{
		std::string path = name.toStdString();

		currentMusic.readCustomMusicCSV(path);
		currentMusic.convertCodeMatricesToArrays();
		if (!isFileLoaded)
			isFileLoaded = true;
	}

	//Counters
	int pulsesElapsed;	int beatsElapsed; 	int barsElapsed;
	int currentMelodicMeasure; 
	int beatsElapsed_withinBar; int barsElapsed_withinMeasure; int measuresElapsed_withinMovement;
	int sixteenthCounter_withinMeasure; int sixteenthCounter_withinBar;

	bool barCompleted = false;
	bool percMeasureCompleted = false;
	bool melMeasureCompleted = false;
	bool fileCompleted = false;

	//Clock Increment
	void incrementPulseCounter();
	short fetchNewMusicInfo(int trackIndex, int infoType, int style);

	//File Switch
	void openNextFile()
	{
		resetCounters();
		if (defaultMode)
		{
			musicIndex_default = (musicIndex_default + 1) % numDefaultFiles;
			currentMusic = defaultMusic[musicIndex_default];
		}
		else
		{
			musicIndex_custom = (musicIndex_custom + 1) % numActiveFiles;
			currentMusic = customMusic[musicIndex_custom];
		}
	}

	bool lastBarCondition = false;
	//Randomized Indices
	int musicIndex_custom = 0;
	int musicIndex_default = 0;
	int compReductionIndex = 0;
	short songStructureIndex = 0;
	int kickIndex;
	int snareIndex;
	int snareFillIndex;
	int hhIndex;
	int riffVelIndex;
	int riffOctIndex;
	int stabsIndex;
	int arpVelIndex;
	int arpDegIndex;
	int crashIndex;
	void randomizeRhythms();
};