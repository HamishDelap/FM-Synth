/*
  ==============================================================================

    OperatorPanel.cpp
    Created: 2 Apr 2025 10:24:08am
    Author:  hamis

  ==============================================================================
*/

#include "OperatorPanel.h"

OperatorPanelComponent::OperatorPanelComponent(FMSynthAudioProcessor& p)
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

    //  Modulator
    addAndMakeVisible(m_modRatioLabel);
    m_modRatioLabel.setFont(juce::Font(24.0f, juce::Font::plain));
    m_modRatioLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(234, 234, 234));
    std::stringstream modIndexStream;
    modIndexStream << std::fixed << std::setprecision(2) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("MOD_RATIO")->load();
    m_modRatioLabel.setText(modIndexStream.str(), juce::dontSendNotification);

    InitSlider(m_modRatioKnob, m_pModRatioAttachment, "MOD_RATIO");
    InitSlider(m_modLevelKnob, m_pModLevelAttachment, "MOD_LEVEL");

    //  Carrier
    addAndMakeVisible(m_carrierWaveformMenu);
    m_carrierWaveformMenu.addItem("Sine", 1);
    m_carrierWaveformMenu.addItem("Triangle", 2);
    m_carrierWaveformMenu.addItem("Square", 3);
    m_carrierWaveformMenu.addItem("Saw", 4);
    m_carrierWaveformMenu.setSelectedId(1);
    m_carrierWaveformMenuAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(m_audioProcessor.m_stateManager.apvt, "CARRIER_WAVEFORM", m_carrierWaveformMenu);

    InitSlider(m_attackKnob, m_pAttackKnobAttachment, "CARRIER_ATTACK");
    InitSlider(m_decayKnob, m_pDecayKnobAttachment, "CARRIER_DECAY");
    InitSlider(m_sustainKnob, m_pSustainKnobAttachment, "CARRIER_SUSTAIN");
    InitSlider(m_releaseKnob, m_pReleaseKnobAttachment, "CARRIER_RELEASE");
}

OperatorPanelComponent::~OperatorPanelComponent()
{}

void OperatorPanelComponent::paint(juce::Graphics&)
{}

void OperatorPanelComponent::resized()
{
    int knobSize = 80;
    m_modLevelKnob.setBounds(49, 106, knobSize, knobSize);
    m_modRatioKnob.setBounds(144, 106, knobSize, knobSize);
    m_modRatioLabel.setBounds(260, 121, 80, 40);

    m_carrierWaveformMenu.setBounds(100, 200, 80, 20);
    m_attackKnob.setBounds(17, 258, knobSize, knobSize);
    m_decayKnob.setBounds(17 + 95, 258, knobSize, knobSize);
    m_sustainKnob.setBounds(17 + 95 * 2, 258, knobSize, knobSize);
    m_releaseKnob.setBounds(17 + 95 * 3, 258, knobSize, knobSize);
}

void OperatorPanelComponent::comboBoxChanged(ComboBox* newComboBox)
{}

void OperatorPanelComponent::sliderValueChanged(Slider* slider)
{
    UpdateLabels();
}

void OperatorPanelComponent::UpdateLabels()
{
    std::stringstream modIndexStream;
    modIndexStream << std::fixed << std::setprecision(2) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("MOD_RATIO")->load();
    m_modRatioLabel.setText(modIndexStream.str(), juce::dontSendNotification);
}
