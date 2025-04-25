/*
  ==============================================================================

    PresetBar.cpp
    Created: 23 Apr 2025 8:39:48pm
    Author:  hamis

  ==============================================================================
*/

#include "PresetBar.h"

PresetBarComponent::PresetBarComponent(JStateManager& stateManager) : m_stateManager(stateManager)
{
	m_saveButton.setClickingTogglesState(false);
	m_saveButton.setTooltip("Save preset");
	m_saveButton.onClick = [this]() {
		
	};
	addAndMakeVisible(m_saveButton);
    
	juce::Image savePassive = juce::ImageFileFormat::loadFrom(BinaryData::SaveButtonPassive_png, BinaryData::SaveButtonPassive_pngSize);
    juce::Image saveHover = juce::ImageFileFormat::loadFrom(BinaryData::SaveButtonHover_png, BinaryData::SaveButtonHover_pngSize);
    juce::Image saveActive = juce::ImageFileFormat::loadFrom(BinaryData::SaveButtonActive_png, BinaryData::SaveButtonActive_pngSize);

	setSaveButtonImages(savePassive, saveHover, saveActive);

    addAndMakeVisible(m_dropdown);
	m_dropdown.setEditableText(true);
    m_dropdown.setSelectedId(1);
    m_dropdown.setLookAndFeel(&m_lookAndFeel);
}

void PresetBarComponent::resized()
{
	auto bounds = getLocalBounds().reduced(4);
	auto iconSize = bounds.getHeight();

	auto saveBounds = bounds;
	m_saveButton.setBounds(saveBounds.removeFromLeft(iconSize).reduced(2));
	bounds.removeFromLeft(iconSize + 6);
	m_dropdown.setBounds(bounds);
}

void PresetBarComponent::paint(juce::Graphics& g)
{
	auto r = getLocalBounds().toFloat();

	// Background
	g.setColour(juce::Colour::fromRGB(37, 39, 43));
	g.fillRoundedRectangle(r, 4.0f);

	// Border
	g.setColour(juce::Colour::fromRGB(60, 60, 60));
	g.drawRoundedRectangle(r.reduced(0.5f), 4.0f, 1.0f);

	// Divider
	float iconWidth = getHeight() - 2;
	g.setColour(juce::Colour::fromRGB(60, 60, 60));
	g.drawLine(iconWidth, 4.0f, iconWidth, getHeight() - 4.0f, 1.0f);
}

void PresetBarComponent::setSaveButtonImages(const juce::Image& normal, const juce::Image& over, const juce::Image& down)
{
	m_saveButton.setImages(false, true, true,
		normal, 1.0f, {},
		over, 1.0f, {},
		down, 1.0f, {});
}
    
void PresetBarComponent::SetCallbacks(OnSaveCallback saveCallback, OnLoadCallback loadCallback)
{
	m_saveCallback = saveCallback;
	m_loadCallback = loadCallback;
}

void PresetBarComponent::PopulateDropdown(std::vector<juce::String>& items)
{
	int id = 1;
	for (auto& item : items)
	{
		m_dropdown.addItem(item, id++);
	}
	m_dropdown.setSelectedId(1);
}

