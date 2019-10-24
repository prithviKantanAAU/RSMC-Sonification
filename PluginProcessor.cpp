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
}

SliderSonificationExpAudioProcessor::~SliderSonificationExpAudioProcessor()
{
}

void SliderSonificationExpAudioProcessor::hiResTimerCallback()
{
	if (taskInProgress)
	{
		timeLeft -= 0.001;
	}
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
			timeLeft = timeLimit;
		}
		break;
	case 2:
		taskInProgress = false;
		storeTaskPerformance();
		interfaceState = 3;
		break;
	case 3:
		storeAestheticRating();
		sonificationsElapsed++;
		timeLeft = timeLimit;
		interfaceState = 2;

		if (sonificationsElapsed == 20)
		{
			saveData();
			break;
		}
		getNewSonificationIndex();
		getNewTargetValue();
		break;
	}
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
	int valuePre = randGen.nextInt(80);
	current_Target = (float)(10 + valuePre) / 100.0;
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

	//mapTargetDistance(0);		//Initialize
}

void SliderSonificationExpAudioProcessor::mapTargetDistance(float sliderValue)
{
	current_ErrorPercent = (sliderValue - current_Target) / current_Target * 100;
	checkOvershoot(current_ErrorPercent);
	errorPercent_Prev = current_ErrorPercent;
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
	std::string path = "D:\\Study\\Semester 3\\RSMC\\Participant " + std::to_string(index) + " .csv";
	while (stat(path.c_str(), &buffer) == 0) // Check File Exists
	{
		index++;
		path = "D:\\Study\\Semester 3\\RSMC\\Participant " + std::to_string(index) + " .csv";
	}

	expOutcomes = fopen(path.c_str(), "w");

	std::string format_Header = "%s,%d,%c,%d,\n";  //Name,Age,Sex,OMSI
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
