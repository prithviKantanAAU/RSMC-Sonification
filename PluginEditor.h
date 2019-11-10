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
	bool isDanish;
	ToggleButton isDanishToggle;
	Label language;
	bool isDanish_z1;
	void handleLanguageChange();
	void setLabels_English();
	void setLabels_Danish();

	//Interface Screen 1
	Label instructions_S1;
	Label nameLabel; TextEditor nameField;
	Label ageLabel;	TextEditor ageField;
	Label mSophLabel; TextEditor mSophField;
	Label genderLabel; TextEditor genderField;

	//Interface Screen 2
	Label trainingOrTask;
	Label closeToTarget;

	Label instructions_S2;
	Label timeLeft;
	Slider mainTaskSlider;
	Label mainTaskSliderLabel;
	TextButton startButton;
	Label targetHint;
	Label pressSpace;

	//Interface Screen 3
	Label errorFeedback;
	Label instructions_S3;
	Slider aestheticRating;
	Label aestheticRatingLabel;
	Slider aestheticRating2;
	Label aestheticRating2Label;

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

	std::string targetHintsEnglish[30] =
	{
		"HINT: The target is the point where melody pitch is LOWEST",
		"HINT: The target is the point where melody is LOUDEST",
		"HINT: The target is the point where melody sounds MOST BRIGHT",
		"HINT: The target is the point where drums are MOST IN SYNC",
		"HINT: The target is the point where the TUNE IS MOST CLEAR",
		"HINT: The target is the point where melody VOLUME IS MOST STABLE",
		"HINT: The target is the point where melody IS LEAST DISTORTED",
		"HINT: The target is the point where the sound effect is NOT HEARD",
		"HINT: The target is the point where the wah wah effect is NOT HEARD",

		"HINT: The target is the point where the wave pitch is LOWEST",
		"HINT: The target is the point where the sound is SOFTEST",
		"HINT: The target is the point where the beeps are FASTEST",
		"HINT: The target is the point where the sound is BRIGHTEST",
		"HINT: The target is the point where the VOLUME IS MOST STABLE",
		"HINT: The target is the point where only ONE beep can be heard",
		"HINT: The target is the point where the sound is least DISSONANT/ROUGH",
		"HINT: The target is the point where the sound is MOST STABLE",
		"HINT: The target is the point where the sound is MOST STABLE",
	};

	std::string targetHintsDanish[30] =
	{
		"ANTYDNING: Maalet er det point, hvor melodien er DYBEST",
		"ANTYDNING: Maalet er det point, hvor melodien er HOEJEST",
		"ANTYDNING: Maalet er det point, hvor melodien lyder LYSEST",
		"ANTYDNING: Maalet er det point, hvor trommerne er MEST SYNKRONISERET",
		"ANTYDNING: Maalet er det point, hvor melodien er KLAREST",
		"ANTYDNING: Maalet er det point, hvor melodiens LYDSTYRKE ER MEST STABIL",
		"ANTYDNING: Maalet er det point, hvor melodien ER MINDST FORVRAENGET",
		"ANTYDNING: Maalet er det point, hvor lydeffekten IKKE er HOERT",
		"ANTYDNING: Maalet er det point, hvor wah-wah effekten IKKE er HOERT",

		"ANTYDNING: Maalet er det point, hvor tonehoejden er LAVEST",
		"ANTYDNING: Maalet er det point, hvor lydstyrken er LAVEST",
		"ANTYDNING: Maalet er det point, hvor bipperne er HURTIGST",
		"ANTYDNING: Maalet er det point, hvor lyden er LYSEST",
		"ANTYDNING: Maalet er det point, hvor lydstyrken er MEST STABIL",
		"ANTYDNING: Maalet er det point, hvor kun EN BIP kan hoeres",
		"ANTYDNING: Maalet er det point, hvor lyden er er MINDST DISSONANT/BARSK",
		"ANTYDNING: Maalet er det point, hvor lyden er MEST STABIL",
		"ANTYDNING: Maalet er det point, hvor lyden er MEST STABIL"
	};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderSonificationExpAudioProcessorEditor)
};
