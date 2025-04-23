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

	m_label.setText("Lead", juce::dontSendNotification);
	m_label.setFont(juce::Font(16.0f, juce::Font::bold));
	m_label.setJustificationType(juce::Justification::centredLeft);
	m_label.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(m_label);

	m_arrow.setImage(getArrowImage());
	m_arrow.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(m_arrow);

	setInterceptsMouseClicks(true, true);
	setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void PresetBarComponent::resized()
{
	auto bounds = getLocalBounds().reduced(4);
	auto iconSize = bounds.getHeight();

	m_saveButton.setBounds(bounds.removeFromLeft(iconSize));
	m_arrow.setBounds(bounds.removeFromRight(iconSize / 2));
	m_label.setBounds(bounds);
}

void PresetBarComponent::paint(juce::Graphics& g)
{
	auto r = getLocalBounds().toFloat();

	// Background
	g.setColour(juce::Colour::fromRGB(37, 39, 43));
	g.fillRoundedRectangle(r, 4.0f);

	// Hover highlight
	if (m_isHovered)
	{
		g.setColour(juce::Colours::white.withAlpha(0.06f));
		g.fillRoundedRectangle(r.reduced(1.0f), 3.0f);
	}

	// Border
	g.setColour(juce::Colour::fromRGB(60, 60, 60));
	g.drawRoundedRectangle(r.reduced(0.5f), 4.0f, 1.0f);

	// Divider
	float iconWidth = getHeight();
	g.setColour(juce::Colour::fromRGB(60, 60, 60));
	g.drawLine(iconWidth, 4.0f, iconWidth, getHeight() - 4.0f, 1.0f);
}

void PresetBarComponent::mouseEnter(const juce::MouseEvent& e)
{
	if (e.getMouseDownPosition().getX() > (getLocalBounds().getHeight() + 4))
	{
		m_isHovered = true;
		repaint();
	}
}

void PresetBarComponent::mouseExit(const juce::MouseEvent& e)
{
	if (e.getMouseDownPosition().getX() > (getLocalBounds().getHeight() + 4))
	{
		m_isHovered = false;
		repaint();
	}
}

void PresetBarComponent::mouseUp(const juce::MouseEvent& e)
{
	if (e.getMouseDownPosition().getX() > (getLocalBounds().getHeight() + 4) && e.mouseWasClicked())
	{
		if (e.mouseWasClicked())
		{
			juce::PopupMenu menu;
			menu.setLookAndFeel(&m_lookAndFeel);
			menu.addItem("Lead", [this]() { setPresetText("Lead"); });
			menu.addItem("Pad", [this]() { setPresetText("Pad"); });
			menu.addItem("Bass", [this]() { setPresetText("Bass"); });

			juce::PopupMenu::Options options;
			menu.showMenuAsync(options);
		}
	}
}

void PresetBarComponent::setPresetText(const juce::String& newName)
{
	m_label.setText(newName, juce::dontSendNotification);
	repaint();
}

void PresetBarComponent::setSaveButtonImages(const juce::Image& normal, const juce::Image& over, const juce::Image& down)
{
	m_saveButton.setImages(false, true, true,
		normal, 1.0f, {},
		over, 1.0f, {},
		down, 1.0f, {});
}

