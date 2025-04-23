/*
  ==============================================================================

    PresetBar.h
    Created: 23 Apr 2025 8:39:48pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "JStateManager.h"
#include "KnobLookAndFeel.h"

class PresetBarComponent : public juce::Component
{
public:
    PresetBarComponent(JStateManager& stateManager);

    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    void setPresetText(const juce::String& newName);
    void setSaveButtonImages(const juce::Image& normal, const juce::Image& over, const juce::Image& down);

private:
    KnobLookAndFeel m_lookAndFeel;
    JStateManager& m_stateManager;
    juce::ImageButton m_saveButton;
    juce::Label m_label;
    juce::ImageComponent m_arrow;
    bool m_isHovered = false;

    juce::Image getArrowImage()
    {
        // Replace with actual arrow image or use SVG if you prefer
        juce::Path p;
        p.addTriangle(0.0f, 0.0f, 6.0f, 0.0f, 3.0f, 6.0f);

        juce::Image img(juce::Image::ARGB, 6, 6, true);
        juce::Graphics g(img);
        g.setColour(juce::Colours::white);
        g.fillPath(p);

        return img;
    }
};

