/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "PresetBar.h";
#include "PanelSelector.h"
#include "OperatorPanel.h"
#include "FilterPanel.h"
#include "EffectsPanel.h"
#include "WaveformPanel.h"

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
    void UpdatePanel(PanelSelector::Panel& activePanel);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthAudioProcessor& m_audioProcessor;
    
    KnobLookAndFeel m_lookAndFeel;

    PanelSelector m_panelSelector;
   
    PresetBarComponent m_presetBar;

    // Panels 
    OperatorPanelComponent m_operatorPanel;
    FilterPanelComponent m_filterPanel;
    EffectsPanelComponent m_effectsPanel;
    WaveformPanelComponent m_waveformPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};
