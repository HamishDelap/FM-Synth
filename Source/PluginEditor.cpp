/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthAudioProcessorEditor::FMSynthAudioProcessorEditor (FMSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), m_audioProcessor (p),
    m_operatorPanel(p), m_filterPanel(p), m_effectsPanel(p), m_waveformPanel(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (516,  516);

    addAndMakeVisible(m_operatorPanel);
    addAndMakeVisible(m_filterPanel);
    addAndMakeVisible(m_effectsPanel);
    addAndMakeVisible(m_waveformPanel);
    m_filterPanel.setVisible(false);
    m_effectsPanel.setVisible(false);
    m_waveformPanel.setVisible(false);

    addAndMakeVisible(m_panelSelector);
    m_panelSelector.SetButtonCallback([this](PanelSelector::Panel panel) {
        UpdatePanel(panel);
	});
}

FMSynthAudioProcessorEditor::~FMSynthAudioProcessorEditor()
{
    m_audioProcessor.m_keyboardState.reset();
}

//==============================================================================
void FMSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    switch (m_panelSelector.GetActivePanel())
    {
    case PanelSelector::Panel::Operators:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::OperatorsPanel_png, BinaryData::OperatorsPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    case PanelSelector::Panel::Filter:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::FilterPanel_png, BinaryData::FilterPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    case PanelSelector::Panel::Effects:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::EffectsPanel_png, BinaryData::EffectsPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    case PanelSelector::Panel::Waveform:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::WaveformPanel_png, BinaryData::WaveformPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    }
}

void FMSynthAudioProcessorEditor::resized()
{
    m_panelSelector.setBounds(57, 35, 190, 38);

    m_operatorPanel.setBounds(57, 72, 401, 371);
	m_filterPanel.setBounds(57, 72, 401, 371);
	m_effectsPanel.setBounds(57, 72, 401, 371);
	m_waveformPanel.setBounds(57, 72, 401, 371);
}

void FMSynthAudioProcessorEditor::UpdatePanel(PanelSelector::Panel& activePanel)
{
    switch (activePanel)
    {
    case PanelSelector::Panel::Operators:
    {
        m_operatorPanel.setVisible(true);
        m_filterPanel.setVisible(false);
        m_effectsPanel.setVisible(false);
        m_waveformPanel.setVisible(false);
        break;
    }
    case PanelSelector::Panel::Filter:
    {
        m_operatorPanel.setVisible(false);
        m_filterPanel.setVisible(true);
        m_effectsPanel.setVisible(false);
        m_waveformPanel.setVisible(false);
        break;
    }
    case PanelSelector::Panel::Effects:
    {
        m_operatorPanel.setVisible(false);
        m_filterPanel.setVisible(false);
        m_effectsPanel.setVisible(true);
        m_waveformPanel.setVisible(false);
        break;
    }
    case PanelSelector::Panel::Waveform:
    {
        m_operatorPanel.setVisible(false);
        m_filterPanel.setVisible(false);
        m_effectsPanel.setVisible(false);
        m_waveformPanel.setVisible(true);
        break;
    }
    }
    repaint();
}

