/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

//==============================================================================
/**
*/
class FMSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::MidiKeyboardStateListener, public juce::Slider::Listener
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
    KnobLookAndFeel m_knobLookAndFeel;
    //  Modulator
    juce::Label m_modRatioLabel;
    juce::Slider m_modRatioKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_modRatioAttachment;

    juce::Slider m_modLevelKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_modLevelAttachment;

    //  Carrier
    juce::Slider m_carrierAttackKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierAttackAttachment;

    juce::Slider m_carrierDecayKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierDecayAttachment;

    juce::Slider m_carrierSustainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierSustainAttachment;

    juce::Slider m_carrierReleaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_carrierReleaseAttachment;

    // Filter
    juce::Label m_filterCutoffLabel;
    juce::Slider m_filterCutoffKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterCutoffAttachment;

    juce::Slider m_filterQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterQAttachment;

    juce::Slider m_filterAttackKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterAttackAttachment;

    juce::Slider m_filterDecayKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterDecayAttachment;

    juce::Slider m_filterSustainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterSustainAttachment;

    juce::Slider m_filterReleaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_filterReleaseAttachment;

    // Reverb
    juce::Slider m_reverbTimeKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverbTimeAttachment;

    juce::Slider m_reverbAmountKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_reverbAmountAttachment;

    // Distortion
    juce::Slider m_distortionKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_distortionAttachment;

    //Waveform Dropdown
    juce::Font m_carrierWaveformFont{ 12.0f };
    juce::ComboBox m_carrierWaveformMenu;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> m_carrierWaveformMenuAttachment;

    juce::MidiKeyboardState m_keyboardState;
    juce::MidiKeyboardComponent m_keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};
