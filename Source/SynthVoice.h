#pragma once
#include <../JuceLibraryCode/JuceHeader.h>

#include "lldsp.h"
#include "lldsp.oscillators.signalgenerator.h"

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

    lldsp::oscillators::SignalGenerator m_oscillator;
    lldsp::ADSR m_envelope;

};
