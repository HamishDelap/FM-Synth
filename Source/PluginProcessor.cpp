/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
FMSynthAudioProcessor::FMSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), m_stateManager(*this)
#endif
{
    m_synthesizer.clearVoices();
    // Create 5 voices.
    for (int i = 0; i < 16; i++) {
        m_synthesizer.addVoice(new SynthVoice());
    }
    // Tidy up unwanted sounds.
    m_synthesizer.clearSounds();
    // Add a new sound.
    m_synthesizer.addSound(new SynthSound());
}

FMSynthAudioProcessor::~FMSynthAudioProcessor()
{
}

//==============================================================================
const juce::String FMSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_keyboardState.reset();
    m_synthesizer.setCurrentPlaybackSampleRate(sampleRate);
    m_midiMessageCollector.reset(sampleRate);

    // Effects
	m_reverb = std::make_unique<lldsp::effects::Reverb>(sampleRate);

    for (int i = 0; i < m_synthesizer.getNumVoices(); i++) {
        // Check that myVoice is a SynthVoice*
        if (SynthVoice* pVoice = dynamic_cast<SynthVoice*>(m_synthesizer.getVoice(i))) {
            pVoice->SetSampleRate(sampleRate);
        }
    }

}

void FMSynthAudioProcessor::releaseResources()
{
    m_keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif
    
void FMSynthAudioProcessor::UpdateVoiceParameters()
{
    SynthVoice::Parameters parameters;
    parameters.modRatio = m_stateManager.apvt.getRawParameterValue("MOD_RATIO")->load();
    parameters.modLevel = m_stateManager.apvt.getRawParameterValue("MOD_LEVEL")->load();
    parameters.modADSRParams.attack = m_stateManager.apvt.getRawParameterValue("MOD_ATTACK")->load();
    parameters.modADSRParams.decay = m_stateManager.apvt.getRawParameterValue("MOD_DECAY")->load();
    parameters.modADSRParams.sustain = m_stateManager.apvt.getRawParameterValue("MOD_SUSTAIN")->load();
    parameters.modADSRParams.release = m_stateManager.apvt.getRawParameterValue("MOD_RELEASE")->load();
    
    parameters.carrierWaveform = static_cast<lldsp::oscillators::Waveform>(m_stateManager.apvt.getRawParameterValue("CARRIER_WAVEFORM")->load());
    parameters.carrierADSRParams.attack = m_stateManager.apvt.getRawParameterValue("CARRIER_ATTACK")->load();
    parameters.carrierADSRParams.decay = m_stateManager.apvt.getRawParameterValue("CARRIER_DECAY")->load();
    parameters.carrierADSRParams.sustain = m_stateManager.apvt.getRawParameterValue("CARRIER_SUSTAIN")->load();
    parameters.carrierADSRParams.release = m_stateManager.apvt.getRawParameterValue("CARRIER_RELEASE")->load();

    parameters.filterCutoff = m_stateManager.apvt.getRawParameterValue("FILTER_CUTOFF")->load();
    parameters.filterQ = m_stateManager.apvt.getRawParameterValue("FILTER_Q")->load();
	parameters.filterADSRParams.attack = m_stateManager.apvt.getRawParameterValue("FILTER_ATTACK")->load();
    parameters.filterADSRParams.decay = m_stateManager.apvt.getRawParameterValue("FILTER_DECAY")->load();
    parameters.filterADSRParams.sustain = m_stateManager.apvt.getRawParameterValue("FILTER_SUSTAIN")->load();
    parameters.filterADSRParams.release = m_stateManager.apvt.getRawParameterValue("FILTER_RELEASE")->load();

    parameters.reverbAmount = m_stateManager.apvt.getRawParameterValue("REVERB_AMOUNT")->load();
    parameters.reverbTime = m_stateManager.apvt.getRawParameterValue("REVERB_TIME")->load();

    parameters.distortionGain = m_stateManager.apvt.getRawParameterValue("DISTORTION_GAIN")->load();

    for (int i = 0; i < m_synthesizer.getNumVoices(); i++)
    {
        // Check that myVoice is a SynthVoice*
        if (SynthVoice* pVoice = dynamic_cast<SynthVoice*>(m_synthesizer.getVoice(i))) {
            pVoice->SetParameters(parameters);
        }
    }
    m_parameters = parameters;
}

void FMSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    UpdateVoiceParameters();

    m_keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    buffer.clear();
    m_synthesizer.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        auto* rChannelData = buffer.getWritePointer(1, 0);
        auto* lChannelData = buffer.getWritePointer(0, 0);
        rChannelData[sampleIndex] = m_reverb->Process(rChannelData[sampleIndex], m_parameters.reverbTime, m_parameters.reverbAmount);
        if (m_parameters.distortionGain > 0)
        {
			rChannelData[sampleIndex] = lldsp::effects::TanhWaveshaper(rChannelData[sampleIndex], m_parameters.distortionGain);
        }
        lChannelData[sampleIndex] = rChannelData[sampleIndex];
    }
}

//==============================================================================
bool FMSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMSynthAudioProcessor::createEditor()
{
    return new FMSynthAudioProcessorEditor (*this);
}

//==============================================================================
void FMSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FMSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMSynthAudioProcessor();
}
