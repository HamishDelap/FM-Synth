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
		auto presets = m_stateManager.getPresets();
		auto text = m_dropdown.getText();
		if (text.isEmpty())
		{
			// Show a modal warning the user
			jassertfalse;
		}
		else if (presets.contains(text, true))
		{
			// Pre-existing preset so maybe confirm overwrite?
			jassertfalse;
		}
		else
		{
			// Write the preset
			m_stateManager.writePreset(m_dropdown.getText());
		}
		PopulateDropdown(presets);
	};
	addAndMakeVisible(m_saveButton);
    
	juce::Image savePassive = juce::ImageFileFormat::loadFrom(BinaryData::SaveButtonPassive_png, BinaryData::SaveButtonPassive_pngSize);
    juce::Image saveHover = juce::ImageFileFormat::loadFrom(BinaryData::SaveButtonHover_png, BinaryData::SaveButtonHover_pngSize);
    juce::Image saveActive = juce::ImageFileFormat::loadFrom(BinaryData::SaveButtonActive_png, BinaryData::SaveButtonActive_pngSize);

	setSaveButtonImages(savePassive, saveHover, saveActive);

	PopulateDropdown(m_stateManager.getPresets());
    addAndMakeVisible(m_dropdown);
	m_dropdown.setEditableText(true);
    m_dropdown.setSelectedId(1);
    m_dropdown.setLookAndFeel(&m_lookAndFeel);
	m_dropdown.onChange = [this]() {
		try
		{
			auto preset = m_dropdownPresets.at(m_dropdown.getSelectedId());
			m_stateManager.readPreset(preset);
		}
		catch (std::out_of_range& ex)
		{
			// Id's have gone out of whack
			jassertfalse;
		}
	};
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

void PresetBarComponent::PopulateDropdown(const juce::StringArray& presets)
{
	m_dropdown.clear(juce::dontSendNotification);
	int id = 1;
	for (auto& item : presets)
	{
		m_dropdownPresets.insert({ id, item });
		m_dropdown.addItem(item, id++);
	}
}

