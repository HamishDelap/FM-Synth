/*
  ==============================================================================

    FilterPanel.h
    Created: 2 Apr 2025 10:24:18am
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

class FilterPanelComponent : public juce::Component, public juce::Slider::Listener
{
public:
    FilterPanelComponent(FMSynthAudioProcessor&);
    ~FilterPanelComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;

private:
    void UpdateLabels();

    FMSynthAudioProcessor& m_audioProcessor;
    
    KnobLookAndFeel m_knobLookAndFeel;

    juce::Label m_cutoffLabel;
    juce::Slider m_cutoffKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pCutoffAttachment;

    juce::Slider m_qKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pQAttachment;

    juce::Slider m_attackKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pAttackAttachment;

    juce::Slider m_decayKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pDecayAttachment;

    juce::Slider m_sustainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pSustainAttachment;

    juce::Slider m_releaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pReleaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterPanelComponent)
};

