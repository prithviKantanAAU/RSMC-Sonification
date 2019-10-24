/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SliderSonificationExpAudioProcessorEditor::SliderSonificationExpAudioProcessorEditor (SliderSonificationExpAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);
	startTimerHz(25);
	configureUI();
	toggleScreenState(1);
}

SliderSonificationExpAudioProcessorEditor::~SliderSonificationExpAudioProcessorEditor()
{
}

//==============================================================================
void SliderSonificationExpAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void SliderSonificationExpAudioProcessorEditor::toggleTaskNotCompletedWarning(bool on)
{
	taskNotCompletedWarning.setVisible(on);
}

void SliderSonificationExpAudioProcessorEditor::configureUI()
{
	addAndMakeVisible(next);
	next.setButtonText("Proceed");
	next.changeWidthToFitText();
	next.onClick = [this]
	{
		if (currentScreenState == 1)
		{
				processor.storeParticipantDetails(nameField.getText(), ageField.getText(), mSophField.getText(), genderField.getText());
		}
		if (currentScreenState == 2 && processor.timeLeft == processor.timeLimit)
		toggleTaskNotCompletedWarning(true);
		processor.handleProceedButton();
	};

	addAndMakeVisible(TaskStatus);
	TaskStatus.setText("Tasks Completed: " 
		+ std::to_string(processor.sonificationsElapsed) + "/" + std::to_string(processor.totalSonifications)
		,dontSendNotification);

	addAndMakeVisible(instructions_S1);
	instructions_S1.setText("Please fill in your personal details:",dontSendNotification);

	addAndMakeVisible(nameLabel);
	nameLabel.setText("Name: ",dontSendNotification);
	addAndMakeVisible(nameField);
	nameLabel.attachToComponent(&nameField, true);

	addAndMakeVisible(ageLabel);
	ageLabel.setText("Age: ", dontSendNotification);
	addAndMakeVisible(ageField);
	ageLabel.attachToComponent(&ageField, true);

	addAndMakeVisible(mSophLabel);
	mSophLabel.setText("OMSI: ", dontSendNotification);
	addAndMakeVisible(mSophField);
	mSophLabel.attachToComponent(&mSophField, true);

	addAndMakeVisible(genderLabel);
	genderLabel.setText("Gender (M/F): ", dontSendNotification);
	addAndMakeVisible(genderField);
	genderLabel.attachToComponent(&genderField, true);

	addAndMakeVisible(instructions_S2);
	instructions_S2.setText("Please attempt to locate the target on the slider.",dontSendNotification);

	addAndMakeVisible(timeLeft);
	timeLeft.setText("Time remaining: " + std::to_string((int)processor.timeLeft) + " sec", dontSendNotification);
	
	addAndMakeVisible(mainTaskSlider);
	mainTaskSlider.setRange(0, 1);
	mainTaskSlider.setNumDecimalPlacesToDisplay(2);
	mainTaskSlider.setValue(0);
	mainTaskSlider.onValueChange = [this]
	{
		if (processor.taskInProgress)
		processor.mapTargetDistance(mainTaskSlider.getValue());
	};
	addAndMakeVisible(mainTaskSliderLabel);
	mainTaskSliderLabel.setText("Adjust:", dontSendNotification);
	mainTaskSliderLabel.attachToComponent(&mainTaskSlider, true);

	addAndMakeVisible(startButton);
	startButton.setButtonText("Begin");
	startButton.changeWidthToFitText();
	startButton.onClick = [this]
	{
		processor.beginSoundTask();
	};

	//Interface Screen 3
	addAndMakeVisible(instructions_S3);
	instructions_S3.setText("Please tell us how pleasant the auditory guidance sounded:",dontSendNotification);

	addAndMakeVisible(aestheticRating);
	aestheticRating.setRange(1, 7, 1);
	aestheticRating.setNumDecimalPlacesToDisplay(0);
	aestheticRating.setValue(4);
	aestheticRating.onValueChange = [this]
	{
		processor.setAestheticRating(aestheticRating.getValue());
	};
	addAndMakeVisible(aestheticRatingLabel);
	aestheticRatingLabel.setText("Pleasantness Rating", dontSendNotification);
	aestheticRatingLabel.attachToComponent(&aestheticRating, true);

	addAndMakeVisible(taskNotCompletedWarning);
	taskNotCompletedWarning.setVisible(false);
	taskNotCompletedWarning.setText("Task not yet completed.", dontSendNotification);
	taskNotCompletedWarning.setColour(Label::textColourId,Colours::red);

	addAndMakeVisible(conclusionLabel);
	conclusionLabel.setVisible(false);
	conclusionLabel.setText("Experiment Complete!", dontSendNotification);
}

void SliderSonificationExpAudioProcessorEditor::timerCallback()
{
	currentScreenState = processor.interfaceState;
	if (currentScreenState != screenState_z1)
		toggleScreenState(currentScreenState);
	screenState_z1 = currentScreenState;
	updateTimeRemaining();

	if (currentScreenState == 2 && processor.timeLeft <= 0)
		processor.handleProceedButton();

	if (processor.sonificationsElapsed >= processor.totalSonifications)
	{
		showConclusionScreen();
	}

	if (taskNotCompletedWarning.isVisible())
	{
		taskWarning_Timeout += 0.04;
		if (taskWarning_Timeout > 3)
		{
			toggleTaskNotCompletedWarning(false);
			taskWarning_Timeout = 0;
		}
	}
}

void SliderSonificationExpAudioProcessorEditor::showConclusionScreen()
{
	next.setVisible(false);
	timeLeft.setVisible(false);
	mainTaskSlider.setVisible(false);
	startButton.setVisible(false);
	instructions_S2.setVisible(false);
	instructions_S3.setVisible(false);
	aestheticRating.setVisible(false);
	aestheticRatingLabel.setVisible(false);
	conclusionLabel.setVisible(true);
}

void SliderSonificationExpAudioProcessorEditor::updateTimeRemaining()
{
	timeLeft.setText("Time remaining: " + std::to_string((int)processor.timeLeft) + " sec", dontSendNotification);
	TaskStatus.setText("Tasks Completed: "
		+ std::to_string(processor.sonificationsElapsed) + "/" + std::to_string(processor.totalSonifications)
		, dontSendNotification);

}

void SliderSonificationExpAudioProcessorEditor::toggleScreenState(short currentState)
{
	switch (currentState)
	{
	case 1:
		toggle_S1(true);
		toggle_S2(false);
		toggle_S3(false);
		break;
	case 2:
		toggle_S1(false);
		toggle_S2(true);
		toggle_S3(false);
		mainTaskSlider.setValue(0, dontSendNotification);
		break;
	case 3:
		toggle_S1(false);
		toggle_S2(false);
		toggle_S3(true);
		aestheticRating.setValue(4, dontSendNotification);
		break;
	}
}

void SliderSonificationExpAudioProcessorEditor::toggle_S1(bool on)
{
	instructions_S1.setVisible(on);
	nameLabel.setVisible(on); 
	nameField.setVisible(on);
	ageLabel.setVisible(on);
	ageField.setVisible(on);
	mSophLabel.setVisible(on);
	mSophField.setVisible(on);
	genderLabel.setVisible(on);
	genderField.setVisible(on);
}

void SliderSonificationExpAudioProcessorEditor::toggle_S2(bool on)
{
	instructions_S2.setVisible(on);
	timeLeft.setVisible(on);
	mainTaskSlider.setVisible(on);
	mainTaskSliderLabel.setVisible(on);
	startButton.setVisible(on);
}

void SliderSonificationExpAudioProcessorEditor::toggle_S3(bool on)
{
	instructions_S3.setVisible(on);
	aestheticRating.setVisible(on);
	aestheticRatingLabel.setVisible(on);
}

void SliderSonificationExpAudioProcessorEditor::resized()
{
	next.setBounds(370,220,80,40);
	TaskStatus.setBounds(600, 20, 160, 40);

	//Interface Screen 1
	instructions_S1.setBounds(50, 50, 600, 40);
	nameField.setBounds(50, 100, 200, 40); genderField.setBounds(450, 100, 200, 40);
	ageField.setBounds(50, 160, 200, 40);
	mSophField.setBounds(50, 220, 80, 40);

	//Interface Screen 2
	instructions_S2.setBounds(50, 50, 600, 40);
	timeLeft.setBounds(50, 90, 600, 40);
	mainTaskSlider.setBounds(350, 150, 400, 40);
	startButton.setBounds(150, 150, 100, 40);

	//Interface Screen 3
	instructions_S3.setBounds(50, 50, 600, 40);
	aestheticRating.setBounds(350, 150, 400, 40);

	conclusionLabel.setBounds(320, 150, 160, 40);
	taskNotCompletedWarning.setBounds(25, 150, 100, 40);
}
