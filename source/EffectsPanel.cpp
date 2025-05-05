/*
  ==============================================================================

    EffectsPanel.cpp
    Created: 2 Apr 2025 10:24:34am
    Author:  hamis

  ==============================================================================
*/

#include "EffectsPanel.h"

EffectsPanelComponent::EffectsPanelComponent(FMSynthAudioProcessor& p)
    : m_audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(401, 371);

    auto InitSlider = [pThis = this](juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, const std::string& paramId) {
        pThis->addAndMakeVisible(slider);
        slider.addListener(pThis);
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setLookAndFeel(&pThis->m_knobLookAndFeel);
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(pThis->m_audioProcessor.m_stateManager.apvt, paramId, slider);
    };

    // Reverb
    InitSlider(m_reverbTimeKnob, m_pReverbTimeAttachment, "REVERB_TIME");
    InitSlider(m_reverbAmountKnob, m_pReverbAmountAttachment, "REVERB_AMOUNT");

    // Distortion
    InitSlider(m_distortionKnob, m_pDistortionAttachment, "DISTORTION_GAIN");
}

EffectsPanelComponent::~EffectsPanelComponent()
{}

void EffectsPanelComponent::paint(juce::Graphics&)
{}

void EffectsPanelComponent::resized()
{
    int knobSize = 80;
    m_distortionKnob.setBounds(158, 95, knobSize, knobSize);

    m_reverbTimeKnob.setBounds(106, 243, knobSize, knobSize);
    m_reverbAmountKnob.setBounds(216, 243, knobSize, knobSize);
}

void EffectsPanelComponent::sliderValueChanged(Slider* slider)
{}
