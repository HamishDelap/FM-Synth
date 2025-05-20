#include "WaveformPanel.h"

WaveformPanelComponent::WaveformPanelComponent(FMSynthAudioProcessor& p) : m_audioProcessor(p)
{
    setSize(401, 371);
    startTimerHz(30);
}

int WaveformPanelComponent::FindRisingZeroCrossing(const float* data, int numSamples)
{
    for (int i = 1; i < numSamples; ++i)
    {
        if (data[i - 1] < 0.0f && data[i] >= 0.0f)
            return i;
    }
    return 0;
}

void WaveformPanelComponent::paint(juce::Graphics& g)
{
    // Simple waveform drawing
    auto fill = juce::Colour::fromRGB(34, 175, 255);
    g.setColour(fill);

    int height = getHeight();
    int width = getWidth();

    auto* channelData = visualBuffer.getReadPointer(0);
    int startIndex = FindRisingZeroCrossing(channelData, visualBuffer.getNumSamples());
    if (startIndex + width > visualBuffer.getNumSamples())
    {
        startIndex = 0;
    }

    for (int i = 1; i < width; ++i)
    {
        int i0 = startIndex + (i - 1);
        int i1 = startIndex + i;
        float x0 = static_cast<float>(i-1);
        float x1 = static_cast<float>(i);
        float y0 = juce::jmap(channelData[i0], -1.0f, 1.0f, static_cast<float>(height), 0.0f);
        float y1 = juce::jmap(channelData[i1], -1.0f, 1.0f, static_cast<float>(height), 0.0f);
        g.drawLine(x0, y0, x1, y1, 2);
    }
}

void WaveformPanelComponent::timerCallback()
{
    if (m_audioProcessor.GetWaveformVisualisationBuffer(visualBuffer))
    {
        repaint();
    }
}

