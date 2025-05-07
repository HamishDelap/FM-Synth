#include "PanelSelector.h"
#include "BinaryData.h"

PanelSelector::PanelSelector()
{
    setSize(180, 36);

    juce::Image operatorPassive = juce::ImageFileFormat::loadFrom(BinaryData::OperatorIndicatorPassive_png, BinaryData::OperatorIndicatorPassive_pngSize);
    juce::Image operatorHover = juce::ImageFileFormat::loadFrom(BinaryData::OperatorIndicatorHover_png, BinaryData::OperatorIndicatorHover_pngSize);
    juce::Image operatorActive = juce::ImageFileFormat::loadFrom(BinaryData::OperatorIndicatorActive_png, BinaryData::OperatorIndicatorActive_pngSize);
    
    juce::Image filterPassive = juce::ImageFileFormat::loadFrom(BinaryData::FilterIndicatorPassive_png, BinaryData::FilterIndicatorPassive_pngSize);
    juce::Image filterHover = juce::ImageFileFormat::loadFrom(BinaryData::FilterIndicatorHover_png, BinaryData::FilterIndicatorHover_pngSize);
    juce::Image filterActive = juce::ImageFileFormat::loadFrom(BinaryData::FilterIndicatorActive_png, BinaryData::FilterIndicatorActive_pngSize);
    
    juce::Image fxPassive = juce::ImageFileFormat::loadFrom(BinaryData::EffectsIndicatorPassive_png, BinaryData::EffectsIndicatorPassive_pngSize);
    juce::Image fxHover = juce::ImageFileFormat::loadFrom(BinaryData::EffectsIndicatorHover_png, BinaryData::EffectsIndicatorHover_pngSize);
    juce::Image fxActive = juce::ImageFileFormat::loadFrom(BinaryData::EffectsIndicatorActive_png, BinaryData::EffectsIndicatorActive_pngSize);
    
    juce::Image waveformPassive = juce::ImageFileFormat::loadFrom(BinaryData::WaveformIndicatorPassive_png, BinaryData::WaveformIndicatorPassive_pngSize);
    juce::Image waveformHover = juce::ImageFileFormat::loadFrom(BinaryData::WaveformIndicatorHover_png, BinaryData::WaveformIndicatorHover_pngSize);
    juce::Image waveformActive = juce::ImageFileFormat::loadFrom(BinaryData::WaveformIndicatorActive_png, BinaryData::WaveformIndicatorActive_pngSize);


    addAndMakeVisible(m_operatorButton);
    m_operatorButton.setImages(true, false, true, 
        operatorPassive, 1, juce::Colours::transparentWhite, 
        operatorHover, 1, juce::Colours::transparentWhite, 
        operatorActive, 1, juce::Colours::transparentWhite, 0);
    m_operatorButton.setClickingTogglesState(true);
    m_operatorButton.setRadioGroupId(1);
    m_operatorButton.onClick = [this] { m_currentPanel = Panel::Operators; m_callback(m_currentPanel); };
    m_operatorButton.setToggleState(true, juce::NotificationType::dontSendNotification);

    addAndMakeVisible(m_filterButton);
    m_filterButton.setImages(true, false, true,
        filterPassive, 1, juce::Colours::transparentWhite, 
        filterHover, 1, juce::Colours::transparentWhite, 
        filterActive, 1, juce::Colours::transparentWhite, 0);
    m_filterButton.setClickingTogglesState(true);
    m_filterButton.setRadioGroupId(1);
    m_filterButton.onClick = [this] { m_currentPanel = Panel::Filter; m_callback(m_currentPanel); };

    addAndMakeVisible(m_fxButton);
    m_fxButton.setImages(true, false, true,
        fxPassive, 1, juce::Colours::transparentWhite, 
        fxHover, 1, juce::Colours::transparentWhite, 
        fxActive, 1, juce::Colours::transparentWhite, 0);
    m_fxButton.setClickingTogglesState(true);
    m_fxButton.setRadioGroupId(1);
    m_fxButton.onClick = [this] { m_currentPanel = Panel::Effects; m_callback(m_currentPanel); };

    addAndMakeVisible(m_waveformButton);
    m_waveformButton.setImages(true, false, true,
        waveformPassive, 1, juce::Colours::transparentWhite, 
        waveformHover, 1, juce::Colours::transparentWhite, 
        waveformActive, 1, juce::Colours::transparentWhite, 0);
    m_waveformButton.setClickingTogglesState(true);
    m_waveformButton.setRadioGroupId(1);
    m_waveformButton.onClick = [this] { m_currentPanel = Panel::Waveform; m_callback(m_currentPanel); };

}

PanelSelector::Panel PanelSelector::GetActivePanel() const
{
    return m_currentPanel;
}

void PanelSelector::paint(juce::Graphics&)
{
}

void PanelSelector::resized()
{
    m_operatorButton.setBounds(6, 8, 30, 30);
    m_filterButton.setBounds(48, 9, 30, 30);
    m_fxButton.setBounds(94, 8, 30, 30);
    m_waveformButton.setBounds(136, 9, 38, 30);
}

void PanelSelector::SetButtonCallback(const OnClickCallback& callback)
{
    m_callback = callback;
}

