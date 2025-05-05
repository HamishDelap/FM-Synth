/*
  ==============================================================================

    PresetBarLookAndFeel.h
    Created: 25 Apr 2025 5:05:43pm
    Author:  hamis

  ==============================================================================
*/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class PresetBarLookAndFeel : public juce::LookAndFeel_V4
{
public:
	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;

	void drawComboBox(juce::Graphics& g, int width, int height, bool,
		int, int, int, int, juce::ComboBox& box) override;

	void drawPopupMenuBackground(juce::Graphics& g, [[maybe_unused]] int width, [[maybe_unused]] int height) override;

	juce::Font getComboBoxFont(juce::ComboBox& box) override;

	void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;

};
