/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthAudioProcessorEditor::FMSynthAudioProcessorEditor (FMSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), m_audioProcessor (p), m_keyboardComponent(p.m_keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1356,  516);
    
    InitialiseControls();

    //addAndMakeVisible(m_keyboardComponent);
    //m_audioProcessor.m_keyboardState.addListener(this);
    //m_keyboardComponent.setAvailableRange(0, 127);

}
    
void FMSynthAudioProcessorEditor::InitialiseControls()
{
    auto InitSlider = [pThis = this](juce::Slider& slider, juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, const std::string& paramId) {
        pThis->addAndMakeVisible(slider);
        slider.addListener(pThis);
        slider.setSliderStyle(juce::Slider::Rotary);
        slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        attachment = new juce::AudioProcessorValueTreeState::SliderAttachment(pThis->m_audioProcessor.m_stateManager.apvt, paramId, slider);
    };

    //  Modulator
    InitSlider(m_modRatioKnob, m_modRatioAttachment, "MOD_RATIO");
    InitSlider(m_modLevelKnob, m_modLevelAttachment, "MOD_LEVEL");

    //  Carrier
    InitSlider(m_carrierAttackKnob, m_carrierAttackAttachment, "CARRIER_ATTACK");
    InitSlider(m_carrierDecayKnob, m_carrierDecayAttachment, "CARRIER_DECAY");
    InitSlider(m_carrierSustainKnob, m_carrierSustainAttachment, "CARRIER_SUSTAIN");
    InitSlider(m_carrierReleaseKnob, m_carrierReleaseAttachment, "CARRIER_RELEASE");

    // Filter
    InitSlider(m_filterCutoffKnob, m_filterCutoffAttachment, "FILTER_CUTOFF");
    InitSlider(m_filterQKnob, m_filterQAttachment, "FILTER_Q"); 

    InitSlider(m_filterAttackKnob, m_filterAttackAttachment, "FILTER_ATTACK");
    InitSlider(m_filterDecayKnob, m_filterDecayAttachment, "FILTER_DECAY");
    InitSlider(m_filterSustainKnob, m_filterSustainAttachment, "FILTER_SUSTAIN");
    InitSlider(m_filterReleaseKnob, m_filterReleaseAttachment, "FILTER_RELEASE");

    // Reverb
    InitSlider(m_reverbTimeKnob, m_reverbTimeAttachment, "REVERB_TIME");
    InitSlider(m_reverbAmountKnob, m_reverbAmountAttachment, "REVERB_AMOUNT");

    // Distortion
    InitSlider(m_distortionKnob, m_distortionAttachment, "DISTORTION_GAIN");

    addAndMakeVisible(m_carrierWaveformMenu);
    m_carrierWaveformMenu.addItem("Sine", 1);
    m_carrierWaveformMenu.addItem("Triangle", 2);
    m_carrierWaveformMenu.addItem("Square", 3);
    m_carrierWaveformMenu.addItem("Saw", 4);
    m_carrierWaveformMenu.setSelectedId(1);
    m_carrierWaveformMenuAttachment = new AudioProcessorValueTreeState::ComboBoxAttachment(m_audioProcessor.m_stateManager.apvt, "CARRIER_WAVEFORM", m_carrierWaveformMenu);

    addAndMakeVisible(m_filterCutoffLabel);
    m_filterCutoffLabel.setFont(juce::Font(24.0f, juce::Font::plain));
    m_filterCutoffLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    std::stringstream cutoffStream;
    cutoffStream << std::fixed << std::setprecision(0) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("FILTER_CUTOFF")->load();
    std::string value = cutoffStream.str();
    m_filterCutoffLabel.setText(value, juce::dontSendNotification);
    
    addAndMakeVisible(m_modRatioLabel);
    m_modRatioLabel.setFont(juce::Font(24.0f, juce::Font::plain));
    m_modRatioLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    std::stringstream modIndexStream;
    modIndexStream << std::fixed << std::setprecision(2) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("MOD_RATIO")->load();
    value = modIndexStream.str();
    m_modRatioLabel.setText(value, juce::dontSendNotification);
}

FMSynthAudioProcessorEditor::~FMSynthAudioProcessorEditor()
{
    m_audioProcessor.m_keyboardState.removeListener(this);
    m_audioProcessor.m_keyboardState.reset();
}

void FMSynthAudioProcessorEditor::UpdateLabels()
{
    std::stringstream cutoffStream;
    cutoffStream << std::fixed << std::setprecision(0) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("FILTER_CUTOFF")->load();
    std::string value = cutoffStream.str();
    m_filterCutoffLabel.setText(value, juce::dontSendNotification);

    std::stringstream modIndexStream;
    modIndexStream << std::fixed << std::setprecision(2) << m_audioProcessor.m_stateManager.apvt.getRawParameterValue("MOD_RATIO")->load();
    value = modIndexStream.str();
    m_modRatioLabel.setText(value, juce::dontSendNotification);
}

//==============================================================================
void FMSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::backgroundv2wgrunge_png, BinaryData::backgroundv2wgrunge_pngSize);
    g.drawImageAt(background, 0, 0);
}

void FMSynthAudioProcessorEditor::resized()
{
    int w = (int)m_keyboardComponent.getKeyWidth() * (7 * 10 + 5), h = 100;
    m_keyboardComponent.setSize(w, h);
    m_keyboardComponent.setCentrePosition(getWidth() / 2, getHeight() - 50);

    const int knobSize = 100;

    int yGap = 75;
    m_modLevelKnob.setBounds(60, 110, knobSize, knobSize);
    m_modRatioKnob.setBounds(170, 110, knobSize, knobSize);
    m_modRatioLabel.setBounds(310, 135, 100, 50);

    //  Carrier
    const int spacing = 98;
    m_carrierAttackKnob.setBounds(60, 330, knobSize, knobSize);
    m_carrierDecayKnob.setBounds(60 + spacing, 330, knobSize, knobSize);
    m_carrierSustainKnob.setBounds(60 + spacing*2, 330, knobSize, knobSize);
    m_carrierReleaseKnob.setBounds(60 + spacing*3, 330, knobSize, knobSize);
   
    m_carrierWaveformMenu.setBounds(130, 291, 80, 20);

    // Filter
    m_filterCutoffLabel.setBounds(780, 190, 100, 50);
    m_filterQKnob.setBounds(526, 170, knobSize, knobSize);
    m_filterCutoffKnob.setBounds(526 + 110, 170, knobSize, knobSize);
    m_filterAttackKnob.setBounds(526, 300, 80, 80);
    m_filterDecayKnob.setBounds(526 + 84, 300, 80, 80);
    m_filterSustainKnob.setBounds(526 + 84 * 2, 300, 80, 80);
    m_filterReleaseKnob.setBounds(526 + 84 * 3, 300, 80, 80);

    // Distortion
    m_distortionKnob.setBounds(985, 110, knobSize, knobSize);
    
    // Reverb
    m_reverbTimeKnob.setBounds(1110, 110, knobSize, knobSize);
    m_reverbAmountKnob.setBounds(1210, 110, knobSize, knobSize);
}

void FMSynthAudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
}

void FMSynthAudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
}

void FMSynthAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    UpdateLabels();
}
