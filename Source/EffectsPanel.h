/*
  ==============================================================================

    EffectsPanel.h
    Created: 2 Apr 2025 10:24:34am
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

class EffectsPanelComponent : public juce::Component, public juce::Slider::Listener
{
public:
    EffectsPanelComponent(FMSynthAudioProcessor&);
    ~EffectsPanelComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;

private:
    FMSynthAudioProcessor& m_audioProcessor;
    
    KnobLookAndFeel m_knobLookAndFeel;

    // Distortion
    juce::Slider m_distortionKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pDistortionAttachment;

    // Reverb
    juce::Slider m_reverbTimeKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pReverbTimeAttachment;

    juce::Slider m_reverbAmountKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pReverbAmountAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectsPanelComponent)
};


