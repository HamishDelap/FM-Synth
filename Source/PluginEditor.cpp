/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthAudioProcessorEditor::FMSynthAudioProcessorEditor (FMSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(p.m_keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    addAndMakeVisible(keyboardComponent);
    audioProcessor.m_keyboardState.addListener(this);
    keyboardComponent.setAvailableRange(0, 127);
}

FMSynthAudioProcessorEditor::~FMSynthAudioProcessorEditor()
{
    audioProcessor.m_keyboardState.reset();
}

//==============================================================================
void FMSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FMSynthAudioProcessorEditor::resized()
{
    int w = (int)keyboardComponent.getKeyWidth() * (7 * 10 + 5), h = 100;
    keyboardComponent.setSize(w, h);
    keyboardComponent.setCentrePosition(getWidth() / 2, getHeight() - 50);
}

void FMSynthAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
}

void FMSynthAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
}

