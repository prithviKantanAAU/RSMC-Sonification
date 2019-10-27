/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspFaust.h"
#include "Sequencer.h"
#include "mixerSettings.h"
#include "FaustStrings.h"
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
	
	// Playback Controls //
	void togglePlayPause();
	void stopMusic();

	void hiResTimerCallback();

	float tempo = 0;
	void setTempo(float value)
	{
		tempo = value;
		float intervalMs = 60000 / tempo * 0.25;
		interPulseIntervalMs = (int)intervalMs;
		dspFaust.setParamValue(faustStrings.Tempo.c_str(), value);
	};

	int interPulseIntervalMs = 0.0; float timeElapsed = 0.0; int pulsesElapsed = 0;
	float lastPulseTime = 0.0; float nextPulseTime = 0.0; bool clockTriggeredLast = false;

	void initializeClocking();
	bool checkIfPulseDue()
	{
		if (timeElapsed >= nextPulseTime)
			return true;
		else return false;
	};
	void triggerClock(bool polarity);
	void clockCallback();

	// Music Mapping

	MixerSettings mixerSettings;
	DspFaust dspFaust;
	void onStartMusic();
	void setFilename(String name);
	void initializeTrackGains();
	void setTrackGains(int trackIndex, float value);
	void setTrackMutes(int trackIndex, int value);

	// Music Sequencing

	Sequencer sequencer;
	void handleNewClockPulse();
	void applyTimingSwing();
	//void applyVelocitySwing(int value, float percent);

	// Current Music Info // 0 : Vel // 1 : Scale Degree // 2 : Octave (Mel) OR Type (Chord) 
	short chordInfo[3] = { 0 };
	short chordStabsInfo[3] = { 0 };
	short riffInfo[3] = { 0 };
	short melInfo[3] = { 0 };
	short percInfo[4] = { 0 }; // 0 : Kick // 1 : Snare // 2 : HH	

	void fetchNewMusicInfo();
	void mapNewMusicInfo();
	void toggleTrackMuteManual(bool muted, short trackNum);
	short muteValuesManual[8] = { 0 };

	void applyMasterGain(float value);
	short currentStyle = 0;
	void applyCurrentStyle(short value);
	void applyCurrentStyleGains();
	void applyCurrentStyleEQ();
	void applyCurrentStyleCompSettings();

	//Sonification
	bool paramMapped[10][3] = { false };
	void setParamOn(int category, int index, bool value);
	void mapDistance(float value);

	//Sonification
	short numCategories_Targetless = 3;
	short numSonificationsPerCategory = 10;
	float currentDistance = 0;

	FaustStrings faustStrings;

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
	short totalSonifications = 18;
	short sonificationsElapsed = 0;
	void handleProceedButton();
	
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
	void chooseRandomMusicFile();
	void mapTargetDistance(float sliderValue);
	void storeParticipantDetails(String name, String age, String omsi, String gender);
	void storeTaskPerformance();
	void setAestheticRating(float sliderValue);
	void storeAestheticRating();
	void saveData();

	KeyPress spaceBarContinue;

	//Sonification Task Specific
	void initializeMapping();
	std::string currentMappingString = "";
	bool isPositivePolarityTraditional[18] = { true, true, true, true, true, true, true, true, true };
	bool isPositivePolarityMusical[18] = { true, true, true, true, true, true, true, false, false };

	//String filePath = "D:\\GaitSonification\\MusicCSVs\\Test 11 Mel.csv";
	String filePath = "";
	File currentMelFile;
	String csvNames[5] = {"Test 11 Mel.csv","Test 12 Mel.csv","Test 13 Mel.csv",
						   "Test 14 Mel.csv", "Test 16 Mel.csv"};

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderSonificationExpAudioProcessor)
};
