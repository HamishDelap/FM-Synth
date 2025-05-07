#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class PanelSelector : public juce::Component
{
public:
    PanelSelector();
    void paint(juce::Graphics&) override;
    void resized() override;

    enum class Panel {
        Operators = 0,
        Filter    = 1,
        Effects   = 2,
        Waveform  = 3
    };
    Panel GetActivePanel() const;

    using OnClickCallback = std::function<void(Panel)>;
    void SetButtonCallback(const OnClickCallback& callback);

private:
    OnClickCallback m_callback;
    Panel m_currentPanel = Panel::Operators;
    
    juce::ImageButton m_operatorButton;
    juce::ImageButton m_filterButton;
    juce::ImageButton m_fxButton;
    juce::ImageButton m_waveformButton;

};
