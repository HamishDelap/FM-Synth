/*
  ==============================================================================

    FilterPanel.cpp
    Created: 2 Apr 2025 10:24:18am
    Author:  hamis

  ==============================================================================
*/

#include "FilterPanel.h"

FilterPanelComponent::FilterPanelComponent(FMSynthAudioProcessor& p)
    : m_audioProcessor(p)
{
    setSize(401, 371);

    auto InitSlider = [pThis = this](juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, const std::string& paramId) {
        pThis->addAndMakeVisible(slider);
        slider.addListener(pThis);
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setLookAndFeel(&pThis->m_knobLookAndFeel);
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(pThis->m_audioProcessor.m_stateManager.apvt, paramId, slider);
    };

    // Filter
    addAndMakeVisible(m_cutoffLabel);
    m_cutoffLabel.setFont(juce::Font(24.0f, juce::Font::plain));
    m_cutoffLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(234, 234, 234));
    std::stringstream cutoffStream;
    cutoffStream << std::fixed << std::setprecision(0) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("FILTER_CUTOFF")->load();
    std::string value = cutoffStream.str();
    m_cutoffLabel.setText(value, juce::dontSendNotification);

    InitSlider(m_qKnob, m_pQAttachment, "FILTER_Q");
    InitSlider(m_cutoffKnob, m_pCutoffAttachment, "FILTER_CUTOFF");

    InitSlider(m_attackKnob, m_pAttackAttachment, "FILTER_ATTACK");
    InitSlider(m_decayKnob, m_pDecayAttachment, "FILTER_DECAY");
    InitSlider(m_sustainKnob, m_pSustainAttachment, "FILTER_SUSTAIN");
    InitSlider(m_releaseKnob, m_pReleaseAttachment, "FILTER_RELEASE");
}

FilterPanelComponent::~FilterPanelComponent()
{}

void FilterPanelComponent::paint(juce::Graphics&)
{}

void FilterPanelComponent::resized()
{
    int knobSize = 80;
    m_qKnob.setBounds(49, 106, knobSize, knobSize);
    m_cutoffKnob.setBounds(144, 106, knobSize, knobSize);
    m_cutoffLabel.setBounds(260, 121, 80, 40);

    m_attackKnob.setBounds(17, 258, knobSize, knobSize);
    m_decayKnob.setBounds(17 + 95, 258, knobSize, knobSize);
    m_sustainKnob.setBounds(17 + 95 * 2, 258, knobSize, knobSize);
    m_releaseKnob.setBounds(17 + 95 * 3, 258, knobSize, knobSize);
}

void FilterPanelComponent::sliderValueChanged(Slider* slider)
{
    UpdateLabels();
}

void FilterPanelComponent::UpdateLabels()
{
    std::stringstream cutoffStream;
    cutoffStream << std::fixed << std::setprecision(0) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("FILTER_CUTOFF")->load();
    std::string value = cutoffStream.str();
    m_cutoffLabel.setText(value, juce::dontSendNotification);
}


