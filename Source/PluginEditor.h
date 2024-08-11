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
class FMSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::MidiKeyboardState, juce::MidiKeyboardStateListener, public juce::Slider::Listener
{
public:
    FMSynthAudioProcessorEditor (FMSynthAudioProcessor&);
    ~FMSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    void sliderValueChanged(Slider* slider) override;

private:

    void InitialiseControls();
    void UpdateLabels();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthAudioProcessor& m_audioProcessor;

    // Controls
    //  Modulator
    juce::Label m_modRatioLabel;
    juce::Slider m_modRatioKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_modRatioAttachment;

    juce::Slider m_modLevelKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_modLevelAttachment;

    //  Carrier
    juce::Slider m_carrierAttackKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierAttackAttachment;

    juce::Slider m_carrierDecayKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierDecayAttachment;

    juce::Slider m_carrierSustainKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierSustainAttachment;

    juce::Slider m_carrierReleaseKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierReleaseAttachment;

    // Filter
    juce::Label m_filterCutoffLabel;
    juce::Slider m_filterCutoffKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterCutoffAttachment;

    juce::Slider m_filterQKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterQAttachment;

    juce::Slider m_filterAttackKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterAttackAttachment;

    juce::Slider m_filterDecayKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterDecayAttachment;

    juce::Slider m_filterSustainKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterSustainAttachment;

    juce::Slider m_filterReleaseKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterReleaseAttachment;

    // Reverb
    juce::Slider m_reverbTimeKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverbTimeAttachment;

    juce::Slider m_reverbAmountKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverbAmountAttachment;

    // Distortion
    juce::Slider m_distortionKnob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_distortionAttachment;

    //Waveform Dropdown
    juce::Font m_carrierWaveformFont{ 12.0f };
    juce::ComboBox m_carrierWaveformMenu;
    ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> m_carrierWaveformMenuAttachment;

    juce::MidiKeyboardState m_keyboardState;
    juce::MidiKeyboardComponent m_keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};
