/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FMSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::MidiKeyboardState, juce::MidiKeyboardStateListener
{
public:
    FMSynthAudioProcessorEditor (FMSynthAudioProcessor&);
    ~FMSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthAudioProcessor& audioProcessor;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};
