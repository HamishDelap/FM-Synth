#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

#include "PresetBar.h"
#include "PanelSelector.h"
#include "OperatorPanel.h"
#include "FilterPanel.h"
#include "EffectsPanel.h"
#include "WaveformPanel.h"
#include "VUComponent.h"

//==============================================================================
/**
*/
class FMSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FMSynthAudioProcessorEditor (FMSynthAudioProcessor&);
    ~FMSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void UpdatePanel(const PanelSelector::Panel& activePanel);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthAudioProcessor& m_audioProcessor;

    OperatorPanelComponent m_operatorPanel;
    FilterPanelComponent m_filterPanel;
    EffectsPanelComponent m_effectsPanel;
    WaveformPanelComponent m_waveformPanel;

    PresetBarComponent m_presetBar;

    VUComponent m_vuMeter;

    KnobLookAndFeel m_lookAndFeel;

    PanelSelector m_panelSelector;
   

    // Panels 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};
