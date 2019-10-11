/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <string>

//==============================================================================
/**
*/
class SliderSonificationExpAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    SliderSonificationExpAudioProcessorEditor (SliderSonificationExpAudioProcessor&);
    ~SliderSonificationExpAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SliderSonificationExpAudioProcessor& processor;
	void timerCallback();

	TextButton next;
	Label TaskStatus;

	//Interface Screen 1
	Label instructions_S1;
	Label nameLabel; TextEditor nameField;
	Label ageLabel;	TextEditor ageField;
	Label mSophLabel; TextEditor mSophField;

	//Interface Screen 2
	Label instructions_S2;
	Label timeLeft;
	Slider mainTaskSlider;
	Label mainTaskSliderLabel;
	TextButton startButton;

	//Interface Screen 3
	Label instructions_S3;
	Slider aestheticRating;
	Label aestheticRatingLabel;

	Label taskNotCompletedWarning;
	void toggleTaskNotCompletedWarning(bool on);
	float taskWarning_Timeout = 0;
	Label conclusionLabel;

	short currentScreenState = 1;
	short screenState_z1 = 0;
	void configureUI();
	void updateTimeRemaining();
	void toggleScreenState(short newState);
	void toggle_S1(bool state);
	void toggle_S2(bool state);
	void toggle_S3(bool state);
	void showConclusionScreen();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderSonificationExpAudioProcessorEditor)
};
