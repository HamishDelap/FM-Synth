/*
  ==============================================================================

    VUComponent.h
    Created: 28 Apr 2025 12:36:19pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PluginProcessor.h"

class VUComponent : public juce::Component, private juce::Timer
{
public:
    VUComponent(FMSynthAudioProcessor&);
    ~VUComponent() override;

    void paint(juce::Graphics&) override;
    void timerCallback() override;

    static float dBFSToMeterLevel(float);
    std::pair<float, float> GetMeterLevels();

private:
    FMSynthAudioProcessor& m_audioProcessor;
    std::array<juce::Image, 11> m_levelImages;

    juce::AudioBuffer<float> m_meterBuffer{2, 2048};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VUComponent)
};

