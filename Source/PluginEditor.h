/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "OperatorPanel.h"
#include "FilterPanel.h"
#include "EffectsPanel.h"

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
    void UpdatePanel();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMSynthAudioProcessor& m_audioProcessor;
    
    KnobLookAndFeel m_lookAndFeel;

    enum class Panel {
        Operators = 0,
        Filter    = 1,
        Effects   = 2
    };
    Panel m_currentPanel = Panel::Operators;

    juce::ImageButton m_nextBtn;
    juce::ImageButton m_prevBtn;
    
    // Panels 
    OperatorPanelComponent m_operatorPanel;
    FilterPanelComponent m_filterPanel;
    EffectsPanelComponent m_effectsPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessorEditor)
};
