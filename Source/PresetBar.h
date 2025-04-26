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
#include "PresetBarLookAndFeel.h"

class PresetBarComponent : public juce::Component
{
public:
    PresetBarComponent(JStateManager& stateManager);

    void resized() override;
    void paint(juce::Graphics& g) override;
    
    void setSaveButtonImages(const juce::Image& normal, const juce::Image& over, const juce::Image& down);

    using OnSaveCallback = std::function<void(juce::String&)>;
    using OnLoadCallback = std::function<void(juce::String&)>;

    void PopulateDropdown(const juce::StringArray& presets);

private:
    JStateManager& m_stateManager;
    OnSaveCallback m_saveCallback;
    OnLoadCallback m_loadCallback;

    std::map<int, juce::String> m_dropdownPresets;

    PresetBarLookAndFeel m_lookAndFeel;
    juce::ImageButton m_saveButton;
    juce::ComboBox m_dropdown;

};

