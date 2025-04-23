/*
  ==============================================================================

    WaveformProcessor.h
    Created: 22 Apr 2025 4:15:14pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveformProcessor
{
public:
    WaveformProcessor(double sampleRate, double bufferTime, int numChannels = 1);
    
    void PushBuffer(juce::AudioBuffer<float>& inBuffer);
    bool PullBuffer(juce::AudioBuffer<float>& outBuffer);

private:
    int m_numChannels = 0;
    int m_bufferSize = 0;
    juce::AbstractFifo m_fifo;
    juce::OwnedArray<juce::AudioBuffer<float>> m_buffer;
};
