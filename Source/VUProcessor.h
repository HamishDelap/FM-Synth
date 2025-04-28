/*
  ==============================================================================

    VUProcessor.h
    Created: 28 Apr 2025 12:36:05pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class VUProcessor
{
public:
    VUProcessor(double sampleRate, double bufferTime, int numChannels = 1);
    
    void PushBuffer(juce::AudioBuffer<float>& inBuffer);
    bool PullBuffer(juce::AudioBuffer<float>& outBuffer);

private:
    int m_numChannels = 0;
    int m_bufferSize = 0;
    juce::AbstractFifo m_fifo;
    juce::AudioBuffer<float> m_buffer;
};
