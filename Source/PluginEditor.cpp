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

    juce::Colour transparent = juce::Colour::fromRGBA(0, 0, 0, 0);
	juce::Image nextNormal = juce::ImageCache::getFromMemory(BinaryData::NextDefault_png, BinaryData::NextDefault_pngSize);
	juce::Image nextOver = juce::ImageCache::getFromMemory(BinaryData::NextHover_png, BinaryData::NextHover_pngSize);
	juce::Image nextPressed = juce::ImageCache::getFromMemory(BinaryData::NextPressed_png, BinaryData::NextPressed_pngSize);
    
    m_operatorPanelIndicator.setImage(juce::ImageCache::getFromMemory(BinaryData::OperatorPanelIndicator_png, BinaryData::OperatorPanelIndicator_pngSize));
    m_filterPanelIndicator.setImage(juce::ImageCache::getFromMemory(BinaryData::FilterPanelIndicator_png, BinaryData::FilterPanelIndicator_pngSize));
    m_effectsPanelIndicator.setImage(juce::ImageCache::getFromMemory(BinaryData::EffectsPanelIndicator_png, BinaryData::EffectsPanelIndicator_pngSize));
    
    addAndMakeVisible(m_operatorPanelIndicator);
    addAndMakeVisible(m_filterPanelIndicator);
    addAndMakeVisible(m_effectsPanelIndicator);
    m_filterPanelIndicator.setVisible(false);
    m_effectsPanelIndicator.setVisible(false);


    m_nextBtn.setImages(true, true, true,
        nextNormal, 1.0f, transparent,
        nextOver, 1.0f, transparent,
        nextPressed, 1.0f, transparent);
    
    m_nextBtn.onClick = [&]() {
        m_currentPanel = (int)m_currentPanel == 2 ? Panel::Operators : (Panel)((int)m_currentPanel + 1);
        UpdatePanel();
    };

    addAndMakeVisible(m_nextBtn);

	juce::Image prevNormal = juce::ImageCache::getFromMemory(BinaryData::PrevDefault_png, BinaryData::PrevDefault_pngSize);
	juce::Image prevOver = juce::ImageCache::getFromMemory(BinaryData::PrevHover_png, BinaryData::PrevHover_pngSize);
	juce::Image prevPressed = juce::ImageCache::getFromMemory(BinaryData::PrevPressed_png, BinaryData::PrevPressed_pngSize);

    m_prevBtn.setImages(true, true, true,
        prevNormal, 1.0f, transparent,
        prevOver, 1.0f, transparent,
        prevPressed, 1.0f, transparent);

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
    m_prevBtn.setBounds(170 - 10, 30, 40, 20);
    m_nextBtn.setBounds(346 - 10, 30, 40, 20);

    m_operatorPanel.setBounds(57, 72, 401, 371);
	m_filterPanel.setBounds(57, 72, 401, 371);
	m_effectsPanel.setBounds(57, 72, 401, 371);
    
    m_operatorPanelIndicator.setBounds(208, 48, 100, 30);
	m_filterPanelIndicator.setBounds(208, 48, 100, 30);
	m_effectsPanelIndicator.setBounds(208, 48, 100, 30);
}

void FMSynthAudioProcessorEditor::UpdatePanel()
{
    switch (m_currentPanel)
    {
    case Panel::Operators:
    {
        m_operatorPanel.setVisible(true);
        m_operatorPanelIndicator.setVisible(true);
        m_filterPanel.setVisible(false);
        m_filterPanelIndicator.setVisible(false);
        m_effectsPanel.setVisible(false);
        m_effectsPanelIndicator.setVisible(false);
        break;
    }
    case Panel::Filter:
    {
        m_operatorPanel.setVisible(false);
        m_operatorPanelIndicator.setVisible(false);
        m_filterPanel.setVisible(true);
        m_filterPanelIndicator.setVisible(true);
        m_effectsPanel.setVisible(false);
        m_effectsPanelIndicator.setVisible(false);
        break;
    }
    case Panel::Effects:
    {
        m_operatorPanel.setVisible(false);
        m_operatorPanelIndicator.setVisible(false);
        m_filterPanel.setVisible(false);
        m_filterPanelIndicator.setVisible(false);
        m_effectsPanel.setVisible(true);
        m_effectsPanelIndicator.setVisible(true);
        break;
    }
    }
    repaint();
}

