#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

class OperatorPanelComponent : public juce::Component, public juce::ComboBox::Listener, public juce::Slider::Listener
{
public:
    explicit OperatorPanelComponent(FMSynthAudioProcessor&);
    ~OperatorPanelComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox* newComboBox) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    void UpdateLabels(double value);
    
    FMSynthAudioProcessor& m_audioProcessor;
    
    KnobLookAndFeel m_knobLookAndFeel;

    // Modulator
    juce::Label m_modRatioLabel;
    juce::Slider m_modRatioKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pModRatioAttachment;

    juce::Slider m_modLevelKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pModLevelAttachment;
    
    // Carrier Waveform Dropdown
    juce::FontOptions m_carrierWaveformFont{ 12.0f };
    juce::ComboBox m_carrierWaveformMenu;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> m_carrierWaveformMenuAttachment;

    // Carrier Envelope
    juce::Slider m_attackKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pAttackKnobAttachment;

    juce::Slider m_decayKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pDecayKnobAttachment;

    juce::Slider m_sustainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pSustainKnobAttachment;

    juce::Slider m_releaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_pReleaseKnobAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OperatorPanelComponent)
};
