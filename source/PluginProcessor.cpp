#include "PluginProcessor.h"
#include "lldsp.effects.reverb.h"
#include "lldsp.effects.distortion.h"
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
                       ), m_stateManager(*this), m_parameters()
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

void FMSynthAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const juce::String FMSynthAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void FMSynthAudioProcessor::changeProgramName (int /*index*/, const juce::String& /*newName*/)
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

    juce::ignoreUnused(sampleRate, samplesPerBlock);

    for (int i = 0; i < m_synthesizer.getNumVoices(); i++) {
        // Check that myVoice is a SynthVoice*
        if (auto* pVoice = dynamic_cast<SynthVoice*>(m_synthesizer.getVoice(i))) {
            pVoice->SetSampleRate(sampleRate);
        }
    }

    m_pWaveformProcessor = std::make_unique<WaveformProcessor>(sampleRate, 1, 2);
    m_pVUMeterProcessor = std::make_unique<VUProcessor>(sampleRate, 1, 2);
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
    m_parameters.modRatio = m_stateManager.apvt.getRawParameterValue("MOD_RATIO")->load();
    m_parameters.modLevel = m_stateManager.apvt.getRawParameterValue("MOD_LEVEL")->load();
    m_parameters.modADSRParams.attack = m_stateManager.apvt.getRawParameterValue("MOD_ATTACK")->load();
    m_parameters.modADSRParams.decay = m_stateManager.apvt.getRawParameterValue("MOD_DECAY")->load();
    m_parameters.modADSRParams.sustain = m_stateManager.apvt.getRawParameterValue("MOD_SUSTAIN")->load();
    m_parameters.modADSRParams.release = m_stateManager.apvt.getRawParameterValue("MOD_RELEASE")->load();
    
    m_parameters.carrierWaveform = static_cast<lldsp::oscillators::Waveform>(m_stateManager.apvt.getRawParameterValue("CARRIER_WAVEFORM")->load());
    m_parameters.carrierADSRParams.attack = m_stateManager.apvt.getRawParameterValue("CARRIER_ATTACK")->load();
    m_parameters.carrierADSRParams.decay = m_stateManager.apvt.getRawParameterValue("CARRIER_DECAY")->load();
    m_parameters.carrierADSRParams.sustain = m_stateManager.apvt.getRawParameterValue("CARRIER_SUSTAIN")->load();
    m_parameters.carrierADSRParams.release = m_stateManager.apvt.getRawParameterValue("CARRIER_RELEASE")->load();

    m_parameters.filterCutoff = m_stateManager.apvt.getRawParameterValue("FILTER_CUTOFF")->load();
    m_parameters.filterQ = m_stateManager.apvt.getRawParameterValue("FILTER_Q")->load();
	m_parameters.filterADSRParams.attack = m_stateManager.apvt.getRawParameterValue("FILTER_ATTACK")->load();
    m_parameters.filterADSRParams.decay = m_stateManager.apvt.getRawParameterValue("FILTER_DECAY")->load();
    m_parameters.filterADSRParams.sustain = m_stateManager.apvt.getRawParameterValue("FILTER_SUSTAIN")->load();
    m_parameters.filterADSRParams.release = m_stateManager.apvt.getRawParameterValue("FILTER_RELEASE")->load();

    m_parameters.reverbAmount = m_stateManager.apvt.getRawParameterValue("REVERB_AMOUNT")->load();
    m_parameters.reverbTime = m_stateManager.apvt.getRawParameterValue("REVERB_TIME")->load();

    m_parameters.distortionGain = m_stateManager.apvt.getRawParameterValue("DISTORTION_GAIN")->load();

    for (int i = 0; i < m_synthesizer.getNumVoices(); i++)
    {
        // Check that myVoice is a SynthVoice*
        if (auto* pVoice = dynamic_cast<SynthVoice*>(m_synthesizer.getVoice(i))) {
            pVoice->SetParameters(m_parameters);
        }
    }
}

static float CubicClipper(float sample)
{
	if (sample > 1.0f)
		return 2.0f / 3.0f;
	else if (sample < -1.0f)
		return -2.0f / 3.0f;
	else
		return (sample - (sample * sample * sample) / 3.0f);
}

void FMSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    UpdateVoiceParameters();

    buffer.clear();
    m_synthesizer.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        auto* rChannelData = buffer.getWritePointer(1, 0);
        auto* lChannelData = buffer.getWritePointer(0, 0);
        rChannelData[sampleIndex] = static_cast<float>(m_reverb->Process(static_cast<double>(rChannelData[sampleIndex]), m_parameters.reverbTime, m_parameters.reverbAmount));
        if (m_parameters.distortionGain > 0)
        {
			rChannelData[sampleIndex] = static_cast<float>(lldsp::effects::DafxDistortion(static_cast<double>(rChannelData[sampleIndex]), m_parameters.distortionGain));
        }

        // Soft Clipping
        rChannelData[sampleIndex] = CubicClipper(rChannelData[sampleIndex]);
        
        lChannelData[sampleIndex] = rChannelData[sampleIndex];
    }

    if (m_pWaveformProcessor) { m_pWaveformProcessor->PushBuffer(buffer); }
    if (m_pVUMeterProcessor) { m_pVUMeterProcessor->PushBuffer(buffer); }
}

//==============================================================================
bool FMSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMSynthAudioProcessor::createEditor()
{
    return new FMSynthAudioProcessorEditor(*this);
}

//==============================================================================
void FMSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    m_stateManager.writeState(destData);
}

void FMSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    m_stateManager.readState(data, sizeInBytes);
}

bool FMSynthAudioProcessor::GetWaveformVisualisationBuffer(juce::AudioBuffer<float>& outBuffer) const
{
    if (m_pWaveformProcessor)
    {
		return m_pWaveformProcessor->PullBuffer(outBuffer);
    }
    return false;
}

bool FMSynthAudioProcessor::GetVUMeterBuffer(juce::AudioBuffer<float>& outBuffer) const
{
    if (m_pVUMeterProcessor)
    {
		return m_pVUMeterProcessor->PullBuffer(outBuffer);
    }
    return false;
}

//==============================================================================
// This creates new instances of the plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMSynthAudioProcessor();
}

