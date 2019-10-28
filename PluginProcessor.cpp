/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SliderSonificationExpAudioProcessor::SliderSonificationExpAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	startTimer(1);
	setTempo(120);
	initializeClocking();
	chooseRandomMusicFile();
}

SliderSonificationExpAudioProcessor::~SliderSonificationExpAudioProcessor()
{
	dspFaust.stop();
	stopTimer();
}

void SliderSonificationExpAudioProcessor::chooseRandomMusicFile()
{
	filePath = currentMelFile.getSpecialLocation(File::currentApplicationFile).getFullPathName();
	filePath = filePath.upToLastOccurrenceOf("\\", true, false);
	int currentCSVIndex = randGen.nextInt(5);
	String currentFile = csvNames[currentCSVIndex];
	filePath += currentFile;
	setFilename(filePath);
}

void SliderSonificationExpAudioProcessor::initializeClocking()
{
	nextPulseTime += interPulseIntervalMs * 0.001;
}

void SliderSonificationExpAudioProcessor::initializeTrackGains()
{
	applyCurrentStyleGains();
	applyCurrentStyleEQ();
	applyCurrentStyleCompSettings();

	int initialTrackMutes[8] = { 1 };
	for (int i = 0; i <= 7; i++)
	{
		setTrackMutes(i, initialTrackMutes[i]);
	}
}

void SliderSonificationExpAudioProcessor::setTrackGains(int trackIndex, float value)
{
	std::string address = faustStrings.baseName + faustStrings.trackGains[trackIndex];
	dspFaust.setParamValue(address.c_str(), value);
}

void SliderSonificationExpAudioProcessor::setTrackMutes(int trackIndex, int value)
{
	std::string address = faustStrings.baseName + faustStrings.trackMutes[trackIndex];
	dspFaust.setParamValue(address.c_str(), value);
}

void SliderSonificationExpAudioProcessor::applyCurrentStyle(short value)
{
	currentStyle = value;
	applyCurrentStyleGains();
	applyCurrentStyleEQ();
	applyCurrentStyleCompSettings();
}

void SliderSonificationExpAudioProcessor::applyCurrentStyleGains()
{
	std::string address = "";
	float gain = 0;

	for (int i = 0; i < 8; i++)
	{
		address = faustStrings.getTrackGainString(i);
		gain = mixerSettings.trackGains[i][currentStyle];
		dspFaust.setParamValue(address.c_str(), gain);
	}
	dspFaust.setParamValue(faustStrings.MasterGainString.c_str(), -19);
}

void SliderSonificationExpAudioProcessor::applyCurrentStyleEQ()
{
	std::string address = "";
	float value = 0;
	bool toMap = true;

	for (int i = 0; i < 8; i++)				//Track ID
	{
		for (int j = 0; j < 4; j++)			//Filter ID
		{
			for (int k = 0; k < 3; k++)		//Filter Param ID
			{
				toMap = true;

				if (j == 0 || j == 3)
				{
					if (k == 1)
						toMap = false;
				}
				if (toMap)
				{
					address = faustStrings.FetchEQ_String(i, j, k);
					value = mixerSettings.fetchEQValue(currentStyle, i, j, k);
					dspFaust.setParamValue(address.c_str(), value);
				}
			}
		}
	}
}

void SliderSonificationExpAudioProcessor::applyCurrentStyleCompSettings()
{
	std::string address = "";
	float value = 0;
	for (int i = 0; i < 8; i++)		//Track ID
	{
		for (int j = 0; j < 4; j++)		//Param ID
		{
			address = faustStrings.FetchComp_String(i, j);
			value = mixerSettings.fetchCompValue(currentStyle, i, j);
			dspFaust.setParamValue(address.c_str(), value);
		}
	}
}

void SliderSonificationExpAudioProcessor::setFilename(String name)
{
	stopMusic();
	sequencer.loadNewFile(name);
	//togglePlayPause();
}

void SliderSonificationExpAudioProcessor::togglePlayPause()
{
	if (taskInProgress)
	{
		dspFaust.stop();
		taskInProgress = false;
	}
	else
	{
		if (sequencer.isFileLoaded)
		{
			onStartMusic();
			taskInProgress = true;
		}
	}
}

void SliderSonificationExpAudioProcessor::stopMusic()
{
	if (taskInProgress)
	{
		taskInProgress = false;
		dspFaust.stop();
		sequencer.resetCounters();
	}
}


void SliderSonificationExpAudioProcessor::hiResTimerCallback()
{
	if (taskInProgress)
	{
		clockCallback();
		timeElapsed += 0.001;
		pulsesElapsed += 1;
		pulsesElapsed = pulsesElapsed % 100;
		timeLeft -= 0.001;

		if (spaceBarContinue.isKeyCurrentlyDown(KeyPress::spaceKey))
		{
			handleProceedButton();
		}
	}
}

void SliderSonificationExpAudioProcessor::clockCallback()
{
	bool isNextPulseDue = false;
	if (clockTriggeredLast && (timeElapsed - lastPulseTime >= 0.07))
		triggerClock(false);

	if (timeElapsed == 0)
		triggerClock(true);

	isNextPulseDue = checkIfPulseDue();
	if (isNextPulseDue)
		triggerClock(true);
}

void SliderSonificationExpAudioProcessor::onStartMusic()
{
	dspFaust.start();
	initializeTrackGains();
}

void SliderSonificationExpAudioProcessor::applyMasterGain(float value)
{
	std::string address = faustStrings.baseName + faustStrings.MasterVol;
	dspFaust.setParamValue(address.c_str(), value);
}

void SliderSonificationExpAudioProcessor::triggerClock(bool polarity)
{
	std::string masterClockAddress = faustStrings.baseName + faustStrings.MasterClock;
	if (polarity == true)
	{
		//FAUST SET ON
		handleNewClockPulse();
		dspFaust.setParamValue(masterClockAddress.c_str(), 1.0);
		lastPulseTime = nextPulseTime;
		//applyTimingSwing();
		nextPulseTime += interPulseIntervalMs * 0.001;
		clockTriggeredLast = true;
	}
	else
	{
		//FAUST SET OFF
		dspFaust.setParamValue(masterClockAddress.c_str(), 0);
		clockTriggeredLast = false;
	}
}

void SliderSonificationExpAudioProcessor::handleNewClockPulse()
{
	sequencer.incrementPulseCounter();
	fetchNewMusicInfo();
	mapNewMusicInfo();
}

void SliderSonificationExpAudioProcessor::fetchNewMusicInfo()
{
	percInfo[0] = sequencer.fetchNewMusicInfo(1, 1, currentStyle); // Kick  Vel
	percInfo[1] = sequencer.fetchNewMusicInfo(2, 1, currentStyle); // Snare Vel
	percInfo[2] = sequencer.fetchNewMusicInfo(3, 1, currentStyle); // HH    Vel
	percInfo[3] = sequencer.fetchNewMusicInfo(8, 1, currentStyle); // Crash Vel

	chordInfo[0] = sequencer.fetchNewMusicInfo(4, 1, currentStyle); // Chord Vel
	chordInfo[1] = sequencer.fetchNewMusicInfo(4, 2, currentStyle); // Chord Deg
	chordInfo[2] = sequencer.fetchNewMusicInfo(4, 3, currentStyle); // Chord Type

	chordStabsInfo[0] = sequencer.fetchNewMusicInfo(7, 1, currentStyle); // Chord/Arp Vel
	chordStabsInfo[1] = sequencer.fetchNewMusicInfo(7, 2, currentStyle); // Arp Deg
	chordStabsInfo[2] = sequencer.fetchNewMusicInfo(7, 3, currentStyle); // Arp Mode

	riffInfo[0] = sequencer.fetchNewMusicInfo(5, 1, currentStyle); // Riff Vel
	riffInfo[1] = sequencer.fetchNewMusicInfo(5, 2, currentStyle); // Riff Deg
	riffInfo[2] = sequencer.fetchNewMusicInfo(5, 3, currentStyle); // Riff Oct

	melInfo[0] = sequencer.fetchNewMusicInfo(6, 1, currentStyle); // Mel Vel
	melInfo[1] = sequencer.fetchNewMusicInfo(6, 2, currentStyle); // Mel Deg
	melInfo[2] = sequencer.fetchNewMusicInfo(6, 3, currentStyle); // Mel Oct
}

void SliderSonificationExpAudioProcessor::mapNewMusicInfo()
{
	if (sequencer.fileCompleted)
	{
		stopMusic();
	}

	dspFaust.setParamValue(faustStrings.KickV.c_str(), percInfo[0]);
	dspFaust.setParamValue(faustStrings.SnareV.c_str(), percInfo[1]);
	dspFaust.setParamValue(faustStrings.hhV.c_str(), percInfo[2]);
	dspFaust.setParamValue(faustStrings.crashV.c_str(), percInfo[3]);

	dspFaust.setParamValue(faustStrings.CV.c_str(), chordInfo[0]);
	dspFaust.setParamValue(faustStrings.CF.c_str(), chordInfo[1]);
	dspFaust.setParamValue(faustStrings.CT.c_str(), chordInfo[2]);

	dspFaust.setParamValue(faustStrings.CSV.c_str(), chordStabsInfo[0]);
	dspFaust.setParamValue(faustStrings.CSF.c_str(), chordStabsInfo[1]);
	dspFaust.setParamValue(faustStrings.CS_ArpMode.c_str(), chordStabsInfo[2]);

	dspFaust.setParamValue(faustStrings.RV.c_str(), riffInfo[0]);
	dspFaust.setParamValue(faustStrings.RF.c_str(), riffInfo[1]);
	dspFaust.setParamValue(faustStrings.RO.c_str(), riffInfo[2]);

	dspFaust.setParamValue(faustStrings.MV.c_str(), melInfo[0]);
	dspFaust.setParamValue(faustStrings.MF.c_str(), melInfo[1]);
	dspFaust.setParamValue(faustStrings.MO.c_str(), melInfo[2]);

	dspFaust.setParamValue(faustStrings.ActiveScale.c_str(), sequencer.currentScale);
	dspFaust.setParamValue(faustStrings.Tonic.c_str(), sequencer.currentMusic.tonic);
	dspFaust.setParamValue(faustStrings.CurrentStyle.c_str(), currentStyle);

	short muteVal = 0;
	for (int i = 0; i < 8; i++)
	{
		std::string paramAddress = faustStrings.getTrackMuteString(i);
		/*short tempMeasureVal = sequencer.currentMelodicMeasure - 1;
		short muteConfiguration = sequencer.musicStructure.songPatterns
											[sequencer.songStructureIndex][tempMeasureVal];

		muteVal = sequencer.musicStructure.trackMuteConfigurations[muteConfiguration][i];*/

		muteVal = muteValuesManual[i];

		dspFaust.setParamValue(paramAddress.c_str(), muteVal);
	}
}

void SliderSonificationExpAudioProcessor::toggleTrackMuteManual(bool muted, short trackIndex)
{
	int val = muted ? 1 : 0;
	muteValuesManual[trackIndex] = val;
}



void SliderSonificationExpAudioProcessor::handleProceedButton()
{
	if (timeLeft == timeLimit)
		return;

	switch (interfaceState)
	{
	case 1:
		if (participantName != "" && participantAge != 0 && participantMSoph != 0)
		{
			interfaceState = 2;
			 getNewSonificationIndex();
			getNewTargetValue();
			chooseRandomMusicFile();
			timeLeft = timeLimit;
		}
		break;
	case 2:
		taskInProgress = false;
		dspFaust.stop();
		storeTaskPerformance();
		interfaceState = 3;
		break;
	case 3:
		storeAestheticRating();
		sonificationsElapsed++;
		timeLeft = timeLimit;
		interfaceState = 2;

		if (sonificationsElapsed == totalSonifications)
		{
			saveData();
			break;
		}
		getNewSonificationIndex();
		getNewTargetValue();
		chooseRandomMusicFile();
		break;
	}
}

void SliderSonificationExpAudioProcessor::initializeMapping()
{
	std::string currentAddress = "";
	
	float valueToMap = 0;
	for (int i = 0; i < totalSonifications / 2; i++)
	{
		currentAddress = faustStrings.getSonificationStringAddress(i);
		valueToMap = isPositivePolarityMusical[i] ? 0 : 1;
		dspFaust.setParamValue(currentAddress.c_str(),valueToMap);
	}
	if (currentSonificationIndex <= totalSonifications / 2)
	{
		currentDistance = getParamValueFunction(mappingFunctionTypes[currentSonificationIndex - 1], current_Target);
		currentMappingString = faustStrings.getSonificationStringAddress(currentSonificationIndex - 1);
		dspFaust.setParamValue(faustStrings.SonificationTypeToggle.c_str(), 0);
		dspFaust.setParamValue(currentMappingString.c_str(),currentDistance);
	}
	else
	{
		currentDistance = current_Target;
		currentMappingString = faustStrings.getSonificationStringAddress(totalSonifications / 2);
		dspFaust.setParamValue(faustStrings.SonificationTypeToggle.c_str(), 1);
		dspFaust.setParamValue(faustStrings.TraditionalSonificationChoice.c_str()
												, currentSonificationIndex - 1 - totalSonifications / 2);
		dspFaust.setParamValue(currentMappingString.c_str(), currentDistance);
	}

	sequencer.resetCounters();
	
}

void SliderSonificationExpAudioProcessor::getNewSonificationIndex()
{
	int randomIndex = randGen.nextInt(totalSonifications - 1) + 1;
	bool alreadyDone = false;

	for (int i = 0; i < totalSonifications; i++)
	{
		if (randomIndex == sonificationIndexes_Elapsed[i])
			alreadyDone = true;
	} //Check if already done

	while (alreadyDone)
	{
		randomIndex = randGen.nextInt(totalSonifications) + 1;
		alreadyDone = false;
		for (int i = 0; i < totalSonifications; i++)
		{
			if (randomIndex == sonificationIndexes_Elapsed[i])
				alreadyDone = true;
		} 
	}
	currentSonificationIndex = randomIndex;
	sonificationIndexes_Elapsed[sonificationsElapsed] = currentSonificationIndex;
}

void SliderSonificationExpAudioProcessor::getNewTargetValue()
{
	int valuePre = randGen.nextInt(30);
	current_Target = (float)(50 + valuePre) / 100.0;
}

void SliderSonificationExpAudioProcessor::storeParticipantDetails(String name, String age, String omsi, String gender)
{
	if (name != "")
	participantName = name;
	if (age != "")
	participantAge = age.getIntValue();
	if (omsi != "")
	participantMSoph = omsi.getIntValue();
	if (gender != "")
	participantGender = gender;
}

void SliderSonificationExpAudioProcessor::beginSoundTask()
{
	taskInProgress = true;
	dspFaust.start();
	initializeMapping();
	initializeTrackGains();
	//mapTargetDistance(0);		//Initialize
}

void SliderSonificationExpAudioProcessor::mapTargetDistance(float sliderValue)
{
	float currentError = abs(sliderValue - current_Target);
	current_ErrorPercent = (sliderValue - current_Target) * 100;
	//current_ErrorPercent = currentError * 100;
	checkOvershoot(current_ErrorPercent);
	errorPercent_Prev = current_ErrorPercent;
	currentError = getParamValueFunction(mappingFunctionTypes[currentSonificationIndex - 1],currentError);
	dspFaust.setParamValue(currentMappingString.c_str(),currentError);
}

void SliderSonificationExpAudioProcessor::storeTaskPerformance()
{
	task_ErrorPercent[currentSonificationIndex - 1] = current_ErrorPercent;
	task_timeTaken[currentSonificationIndex - 1] = timeLimit - timeLeft;
	task_numOvershoots[currentSonificationIndex - 1] = current_NumOvershoots;

	current_NumOvershoots = 0;
}

void SliderSonificationExpAudioProcessor::storeAestheticRating()
{
	task_aestheticRatings[currentSonificationIndex - 1] = current_AestheticRating;
}

void SliderSonificationExpAudioProcessor::setAestheticRating(float sliderValue)
{
	current_AestheticRating = sliderValue;
}

void SliderSonificationExpAudioProcessor::saveData()
{
	int index = 1;
	struct stat buffer;
	filePath = currentMelFile.getSpecialLocation(File::currentApplicationFile).getFullPathName();
	filePath = filePath.upToLastOccurrenceOf("\\", true, false);
	std::string path = filePath.toStdString() + "Participant " + std::to_string(index) + ".csv";
	while (stat(path.c_str(), &buffer) == 0) // Check File Exists
	{
		index++;
		path = filePath.toStdString() + "Participant " + std::to_string(index) + ".csv";
	}

	expOutcomes = fopen(path.c_str(), "w");

	std::string format_Header = "%s,%d,%s,%d,\n";  //Name,Age,Sex,OMSI
	std::string format_Body = "%f,%f,%d,%d,\n";			   //ErrorPercent,TimeTaken,NumOvershoots,AestheticRating

	fprintf(expOutcomes, format_Header.c_str(), participantName, participantAge, participantGender, participantMSoph);
	for (int i = 0; i < totalSonifications; i++)
	{
		fprintf(expOutcomes, format_Body.c_str(), 
			task_ErrorPercent[i],task_timeTaken[i],task_numOvershoots[i],task_aestheticRatings[i]);
	}
	fclose(expOutcomes);
}
//==============================================================================
const String SliderSonificationExpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SliderSonificationExpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SliderSonificationExpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SliderSonificationExpAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SliderSonificationExpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SliderSonificationExpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SliderSonificationExpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SliderSonificationExpAudioProcessor::setCurrentProgram (int index)
{
}

const String SliderSonificationExpAudioProcessor::getProgramName (int index)
{
    return {};
}

void SliderSonificationExpAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SliderSonificationExpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SliderSonificationExpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SliderSonificationExpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SliderSonificationExpAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool SliderSonificationExpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SliderSonificationExpAudioProcessor::createEditor()
{
    return new SliderSonificationExpAudioProcessorEditor (*this);
}

//==============================================================================
void SliderSonificationExpAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SliderSonificationExpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SliderSonificationExpAudioProcessor();
}
