#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class VUComponent : public juce::Component, private juce::Timer
{
public:
    explicit VUComponent(FMSynthAudioProcessor&);
    ~VUComponent() override = default;

    void paint(juce::Graphics&) override;
    void timerCallback() override;

    static float dBFSToMeterLevel(float);
    std::pair<float, float> GetMeterLevels() const;

private:
    FMSynthAudioProcessor& m_audioProcessor;
    std::array<juce::Image, 11> m_levelImages;

    juce::AudioBuffer<float> m_meterBuffer{2, 2048};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VUComponent)
};

