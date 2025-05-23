#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class WaveformPanelComponent : public juce::Component, private juce::Timer
{
public:
    WaveformPanelComponent(FMSynthAudioProcessor&);
    ~WaveformPanelComponent() override = default;

    void paint(juce::Graphics&) override;
    void timerCallback() override;

    static int FindRisingZeroCrossing(const float* data, int numSamples);

private:
    FMSynthAudioProcessor& m_audioProcessor;

    juce::AudioBuffer<float> visualBuffer{ 1, 2048};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformPanelComponent)
};
