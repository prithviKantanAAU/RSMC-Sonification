/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

//==============================================================================
/**
*/
class SliderSonificationExpAudioProcessor  : public AudioProcessor, public HighResolutionTimer
{
public:
    //==============================================================================
    SliderSonificationExpAudioProcessor();
    ~SliderSonificationExpAudioProcessor();

	Random randGen;
	FILE *expOutcomes;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//Interface // 3 Screen Types - Details, Sonification Test, Aesthetic Rating
	short interfaceState = 1;
	float timeLimit = 30;
	float timeLeft = 0;
	short currentSonificationIndex = 0;
	short totalSonifications = 20;
	short sonificationsElapsed = 0;
	void handleProceedButton();

	//Callback
	void hiResTimerCallback();
	
	//Personal Details
	String participantName = "";
	String participantGender = "";
	short participantAge = 0;
	short participantMSoph = 0;

	//Sonification Handling
	float task_ErrorPercent[50] = { 0.0 };
	int task_numOvershoots[50] = { 0 };
	float task_timeTaken[50] = { 0.0 };
	int task_aestheticRatings[50] = { 0 };
	int sonificationIndexes_Elapsed[50] = { 0 };

	bool taskInProgress = false;
	float current_Target = 0;
	float current_ErrorPercent = 0;
	int current_NumOvershoots = 0;
	float errorPercent_Prev = 0.0;
	float current_TimeElapsed = 0.0;
	int current_AestheticRating = 0;
	void beginSoundTask();
	void checkOvershoot(float currentSliderValue)
	{
		if (currentSliderValue * errorPercent_Prev < 0)
			current_NumOvershoots++;
	}
	void getNewSonificationIndex();
	void getNewTargetValue();
	void mapTargetDistance(float sliderValue);
	void storeParticipantDetails(String name, String age, String omsi, String gender);
	void storeTaskPerformance();
	void setAestheticRating(float sliderValue);
	void storeAestheticRating();
	void saveData();

	//Music Control

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderSonificationExpAudioProcessor)
};
