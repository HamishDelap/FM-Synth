#pragma once
#include <../JuceLibraryCode/JuceHeader.h>

#include "lldsp.h"
#include "lldsp.oscillators.oscillator.h"
#include "lldsp.dsp.h"
#include "lldsp.effects.reverb.h"
#include "lldsp.effects.distortion.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice() = default;

    // SynthesizerVoice
    bool canPlaySound(juce::SynthesiserSound* sound);

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition);
    void stopNote(float velocity, bool allowTailOff);

    void pitchWheelMoved(int newPitchWheelValue);
    void controllerMoved(int controllerNumber, int newControllerValue);
    float getControllerValue();

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples);

    // SynthVoice
    void SetSampleRate(const double sampleRate);

    struct Parameters
    {
        double modRatio = 1;
        double modLevel = 1;
        lldsp::ADSR::Parameters modADSRParams;
        lldsp::ADSR::Parameters carrierADSRParams;
        lldsp::oscillators::Waveform carrierWaveform;
        double filterCutoff = 1000;
        double filterQ = 0;
        lldsp::ADSR::Parameters filterADSRParams;
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

    Parameters m_parameters;
   
    lldsp::ADSR m_filterEnvelope;
    lldsp::dsp::BiQuadFilter m_filter;

    lldsp::oscillators::Oscillator m_modulator;
    lldsp::ADSR m_modulatorEnvelope;
    lldsp::oscillators::Oscillator m_carrier;
    lldsp::ADSR m_carrierEnvelope;

};
