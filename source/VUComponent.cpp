#include "VUComponent.h"
#include "BinaryData.h"

VUComponent::VUComponent(FMSynthAudioProcessor& p) : m_audioProcessor(p)
{
    setSize(401, 371);
    startTimerHz(30);

    m_levelImages[0] = juce::ImageFileFormat::loadFrom(BinaryData::vu0_png, BinaryData::vu0_pngSize);
    m_levelImages[1] = juce::ImageFileFormat::loadFrom(BinaryData::vu1_png, BinaryData::vu1_pngSize);
    m_levelImages[2] = juce::ImageFileFormat::loadFrom(BinaryData::vu2_png, BinaryData::vu2_pngSize);
    m_levelImages[3] = juce::ImageFileFormat::loadFrom(BinaryData::vu3_png, BinaryData::vu3_pngSize);
    m_levelImages[4] = juce::ImageFileFormat::loadFrom(BinaryData::vu4_png, BinaryData::vu4_pngSize);
    m_levelImages[5] = juce::ImageFileFormat::loadFrom(BinaryData::vu5_png, BinaryData::vu5_pngSize);
    m_levelImages[6] = juce::ImageFileFormat::loadFrom(BinaryData::vu6_png, BinaryData::vu6_pngSize);
    m_levelImages[7] = juce::ImageFileFormat::loadFrom(BinaryData::vu7_png, BinaryData::vu7_pngSize);
    m_levelImages[8] = juce::ImageFileFormat::loadFrom(BinaryData::vu8_png, BinaryData::vu8_pngSize);
    m_levelImages[9] = juce::ImageFileFormat::loadFrom(BinaryData::vu9_png, BinaryData::vu9_pngSize);
    m_levelImages[10] = juce::ImageFileFormat::loadFrom(BinaryData::vu10_png, BinaryData::vu10_pngSize);
}

using MeterDBFSPoint = std::pair<float, float>; // dBFS, meter level
constexpr std::array<MeterDBFSPoint, 8> DBFS_METER_POINTS= {
    MeterDBFSPoint{0.0f, 1.0f},
    MeterDBFSPoint{-10.0f, 0.75f},
    MeterDBFSPoint{-20.0f, 0.5f},
    MeterDBFSPoint{-30.0f, 0.30f},
    MeterDBFSPoint{-40.0f, 0.15f},
    MeterDBFSPoint{-50.0f, 0.075f},
    MeterDBFSPoint{-60.0f, 0.025f},
    MeterDBFSPoint{-100.0f, 0.0f}
};

static float InterpolateMeterLevel(float dbfs, MeterDBFSPoint upperPoint, MeterDBFSPoint lowerPoint)
{
    return upperPoint.second + ((dbfs - upperPoint.first) / (lowerPoint.first - upperPoint.first)) *
        (lowerPoint.second - upperPoint.second);
}

float VUComponent::dBFSToMeterLevel(float dbfsLevel)
{
    for (int i = 1; i < DBFS_METER_POINTS.size(); i++)
    {
        if (dbfsLevel > DBFS_METER_POINTS[i].first)
        {
            return InterpolateMeterLevel(dbfsLevel, DBFS_METER_POINTS[i - 1], DBFS_METER_POINTS[i]);
        }
    }
    return 0.0;
}

std::pair<float, float> VUComponent::GetMeterLevels() const
{
    auto bufferSize = m_meterBuffer.getNumSamples();
    auto leftChannel = m_meterBuffer.getReadPointer(0);
    auto rightChannel = m_meterBuffer.getReadPointer(1);

    float leftPeak = 0;
    float rightPeak = 0;
    for (int i = 0; i < bufferSize; i++)
    {
        if (auto level = abs(leftChannel[i]); level > leftPeak)
        {
            leftPeak = level;
        }
        if (auto level = abs(rightChannel[i]); level > rightPeak)
        {
            rightPeak = level;
        }
    }

    float leftdBFS = leftPeak == 0.0f ? -100.0f : 20.0f * std::log10(leftPeak);
    float rightdBFS = rightPeak == 0.0f ? -100.0f : 20.0f * std::log10(rightPeak);

    float leftMeterLevel = dBFSToMeterLevel(leftdBFS);
    float rightMeterLevel = dBFSToMeterLevel(rightdBFS);

    return { leftMeterLevel, rightMeterLevel };
}

void VUComponent::paint(juce::Graphics& g)
{
    constexpr int leftX = 10;
    constexpr int rightX = 20;
    constexpr int topY = 10;

    auto [leftLevel, rightLevel] = GetMeterLevels();

    leftLevel = (leftLevel > 1.0) ? 10 : floor(leftLevel * 10);
    rightLevel = (rightLevel > 1.0) ? 10 : floor(rightLevel * 10);

    if (leftLevel >= 0 && leftLevel < m_levelImages.size())
        g.drawImageAt(m_levelImages[static_cast<size_t>(leftLevel)], leftX, topY);

    if (rightLevel >= 0 && rightLevel < m_levelImages.size())
        g.drawImageAt(m_levelImages[static_cast<size_t>(rightLevel)], rightX, topY);
}

void VUComponent::timerCallback()
{
    if (m_audioProcessor.GetVUMeterBuffer(m_meterBuffer))
    {
        repaint();
    }
}


