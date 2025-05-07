#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "lldsp.oscillators.oscillator.h"
#include "lldsp.dsp.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice() = default;

    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    float getControllerValue() const;
    void SetSampleRate(const double sampleRate);

    struct Parameters
    {
        double modRatio = 1;
        double modLevel = 1;
        lldsp::dsp::ADSR::Parameters modADSRParams;
        lldsp::dsp::ADSR::Parameters carrierADSRParams;
        lldsp::oscillators::Waveform carrierWaveform;
        double filterCutoff = 1000;
        double filterQ = 0;
        lldsp::dsp::ADSR::Parameters filterADSRParams;
        double reverbTime = 0;
        double reverbAmount = 0;
        double distortionGain = 0;
    };
    void SetParameters(const Parameters& parameters);

private:
    void UpdateFrequency();

    bool m_noteOn = false;
    int m_controllerValue = 126;
    double m_trueFrequency = 0;
    double m_newFrequency = 0;
    double m_pitchBendChange = 0;
    double m_level = 0;
    double m_velocity = 0;

    Parameters m_parameters;
   
    lldsp::dsp::ADSR m_filterEnvelope;
    lldsp::dsp::BiQuadFilter m_filter;

    lldsp::oscillators::Oscillator m_modulator;
    lldsp::dsp::ADSR m_modulatorEnvelope;
    lldsp::oscillators::Oscillator m_carrier;
    lldsp::dsp::ADSR m_carrierEnvelope;

};
