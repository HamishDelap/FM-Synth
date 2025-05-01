/*
  ==============================================================================

    WaveformPanel.h
    Created: 22 Apr 2025 4:14:56pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class WaveformPanelComponent : public juce::Component, private juce::Timer
{
public:
    WaveformPanelComponent(FMSynthAudioProcessor&);
    ~WaveformPanelComponent() override;

    void paint(juce::Graphics&) override;
    void timerCallback() override;

    int FindRisingZeroCrossing(const float* data, int numSamples);

private:
    FMSynthAudioProcessor& m_audioProcessor;

    juce::AudioBuffer<float> visualBuffer{ 1, 2048};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformPanelComponent)
};
