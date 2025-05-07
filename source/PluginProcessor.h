#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include "SynthVoice.h"
#include "JStateManager.h"
#include "WaveformProcessor.h"
#include "VUProcessor.h"
#include "lldsp.effects.reverb.h"

//==============================================================================
/**
*/
class FMSynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FMSynthAudioProcessor();
    ~FMSynthAudioProcessor() override = default;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Only call from UI
    bool GetWaveformVisualisationBuffer(juce::AudioBuffer<float>& outBuffer) const;
    bool GetVUMeterBuffer(juce::AudioBuffer<float>& outBuffer) const;

    juce::MidiKeyboardState m_keyboardState;
    JStateManager m_stateManager;

private:
    void UpdateVoiceParameters();

    juce::MidiMessageCollector m_midiMessageCollector;
    juce::Synthesiser m_synthesizer;
    SynthVoice::Parameters m_parameters;

    std::unique_ptr<lldsp::effects::Reverb> m_reverb;
    std::unique_ptr<WaveformProcessor> m_pWaveformProcessor;
    std::unique_ptr<VUProcessor> m_pVUMeterProcessor;
   

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthAudioProcessor)
};
