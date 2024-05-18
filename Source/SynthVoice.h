#pragma once
#include <../JuceLibraryCode/JuceHeader.h>

#include "lldsp.h"
#include "lldsp.oscillators.oscillator.h"
#include "lldsp.dsp.h"
#include "lldsp.effects.reverb.h"

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

private:
    int m_controllerValue = 126;
    double m_frequency = 0.0f;
    double m_level = 0.0f;

    lldsp::oscillators::Oscillator m_modulator;
    lldsp::ADSR m_modulatorEnvelope;
    lldsp::oscillators::Oscillator m_carrier;
    lldsp::ADSR m_carrierEnvelope;

    std::unique_ptr<lldsp::dsp::BiQuadFilter> m_filter;
    std::unique_ptr<lldsp::effects::Reverb> m_reverb;


};
