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
    m_operatorPanel(p), m_filterPanel(p), m_effectsPanel(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (516,  516);

    addAndMakeVisible(m_operatorPanel);
    addAndMakeVisible(m_filterPanel);
    addAndMakeVisible(m_effectsPanel);
    m_filterPanel.setVisible(false);
    m_effectsPanel.setVisible(false);

    m_nextBtn.setButtonText("Next");

    m_nextBtn.onClick = [&]() {
        m_currentPanel = (int)m_currentPanel == 2 ? Panel::Operators : (Panel)((int)m_currentPanel + 1);
        UpdatePanel();
    };

    addAndMakeVisible(m_nextBtn);

    m_prevBtn.setButtonText("Prev");

    m_prevBtn.onClick = [&]() {
        m_currentPanel = (int)m_currentPanel == 0 ? Panel::Effects : (Panel)((int)m_currentPanel - 1);
        UpdatePanel();
    };

    addAndMakeVisible(m_prevBtn);
}

FMSynthAudioProcessorEditor::~FMSynthAudioProcessorEditor()
{
    m_audioProcessor.m_keyboardState.reset();
}

//==============================================================================
void FMSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    switch (m_currentPanel)
    {
    case Panel::Operators:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::OperatorsPanel_png, BinaryData::OperatorsPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    case Panel::Filter:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::FilterPanel_png, BinaryData::FilterPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    case Panel::Effects:
    {
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
		juce::Image background = juce::ImageCache::getFromMemory(BinaryData::EffectsPanel_png, BinaryData::EffectsPanel_pngSize);
		g.drawImageAt(background, 0, 0);
        break;
    }
    }
}

void FMSynthAudioProcessorEditor::resized()
{
    m_prevBtn.setBounds(170 - 20, 30, 40, 20);
    m_nextBtn.setBounds(346 - 20, 30, 40, 20);

    m_operatorPanel.setBounds(57, 72, 401, 371);
	m_filterPanel.setBounds(57, 72, 401, 371);
	m_effectsPanel.setBounds(57, 72, 401, 371);
}

void FMSynthAudioProcessorEditor::UpdatePanel()
{
    switch (m_currentPanel)
    {
    case Panel::Operators:
    {
        m_operatorPanel.setVisible(true);
        m_filterPanel.setVisible(false);
        m_effectsPanel.setVisible(false);
        break;
    }
    case Panel::Filter:
    {
        m_operatorPanel.setVisible(false);
        m_filterPanel.setVisible(true);
        m_effectsPanel.setVisible(false);
        break;
    }
    case Panel::Effects:
    {
        m_operatorPanel.setVisible(false);
        m_filterPanel.setVisible(false);
        m_effectsPanel.setVisible(true);
        break;
    }
    }
    repaint();
}

