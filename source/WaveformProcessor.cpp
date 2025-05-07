#include "WaveformProcessor.h"

WaveformProcessor::WaveformProcessor(double sampleRate, double bufferTime, int numChannels) : m_fifo(static_cast<int>(bufferTime * sampleRate))
{
    m_numChannels = numChannels;
    m_bufferSize = static_cast<int>(bufferTime * sampleRate);
    for (int i = 0; i < numChannels; ++i)
        m_buffer.add(new juce::AudioBuffer<float>(1, m_bufferSize));
}
    
void WaveformProcessor::PushBuffer(juce::AudioBuffer<float>& inBuffer)
{
    const int numSamplesToWrite = inBuffer.getNumSamples();

    int start1, size1, start2, size2;
    m_fifo.prepareToWrite(numSamplesToWrite, start1, size1, start2, size2);

    if (size1 > 0)
        for (int ch = 0; ch < m_numChannels; ++ch)
            m_buffer[ch]->copyFrom(0, start1, inBuffer, ch, 0, size1);

    if (size2 > 0)
        for (int ch = 0; ch < m_numChannels; ++ch)
            m_buffer[ch]->copyFrom(0, start2, inBuffer, ch, size1, size2);

    m_fifo.finishedWrite(size1 + size2);
}

bool WaveformProcessor::PullBuffer(juce::AudioBuffer<float>& outBuffer)
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
            outBuffer.copyFrom(ch, 0, *m_buffer[ch], ch, start1, size1);
        }
    }

    if (size2 > 0)
    {
        readData = true;
        for (int ch = 0; ch < outBuffer.getNumChannels(); ++ch)
        {
            outBuffer.copyFrom(ch, size1, *m_buffer[ch], ch, start2, size2);
        }
    }

    m_fifo.finishedRead(size1 + size2);
    return readData;
}


