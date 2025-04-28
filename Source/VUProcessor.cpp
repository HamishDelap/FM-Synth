/*
  ==============================================================================

    VUProcessor.cpp
    Created: 28 Apr 2025 12:36:05pm
    Author:  hamis

  ==============================================================================
*/

#include "VUProcessor.h"

VUProcessor::VUProcessor(double sampleRate, double bufferTime, int numChannels) : m_fifo(bufferTime * sampleRate)
{
    m_numChannels = numChannels;
    m_bufferSize = bufferTime * sampleRate;
	m_buffer = juce::AudioBuffer<float>(numChannels, m_bufferSize);
}
    
void VUProcessor::PushBuffer(juce::AudioBuffer<float>& inBuffer)
{
    const int numSamplesToWrite = inBuffer.getNumSamples();

    int start1, size1, start2, size2;
    m_fifo.prepareToWrite(numSamplesToWrite, start1, size1, start2, size2);

    if (size1 > 0)
        for (int ch = 0; ch < m_numChannels; ++ch)
            m_buffer.copyFrom(ch, start1, inBuffer, ch, 0, size1);

    if (size2 > 0)
        for (int ch = 0; ch < m_numChannels; ++ch)
            m_buffer.copyFrom(ch, start2, inBuffer, ch, size1, size2);

    m_fifo.finishedWrite(size1 + size2);
}

bool VUProcessor::PullBuffer(juce::AudioBuffer<float>& outBuffer)
{
    bool readData = false;
    const int numSamples = outBuffer.getNumSamples();

    int start1, size1, start2, size2;
    m_fifo.prepareToRead(numSamples, start1, size1, start2, size2);

    if (size1 > 0)
    {
        readData = true;
        for (int ch = 0; ch < outBuffer.getNumChannels(); ++ch)
        {
            outBuffer.copyFrom(ch, 0, m_buffer, ch, start1, size1);
        }
    }

    if (size2 > 0)
    {
        readData = true;
        for (int ch = 0; ch < outBuffer.getNumChannels(); ++ch)
        {
            outBuffer.copyFrom(ch, size1, m_buffer, ch, start2, size2);
        }
    }

    m_fifo.finishedRead(size1 + size2);
    return readData;
}



