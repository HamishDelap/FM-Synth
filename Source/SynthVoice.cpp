#pragma once
#include "SynthVoice.h"
#include "SynthSound.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	// If the sound can be cast to the class SynthSound it will return true, otherwise it will return false. (Taken from a tutorial)
	return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	m_frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	m_level = velocity * 0.15;
	m_envelope.SetParameters({ 0.1, 0.1, 1.0, 0.1 });
	m_envelope.NoteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	m_envelope.NoteOff();
	clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	m_controllerValue = newControllerValue;
}

float SynthVoice::getControllerValue()
{
	return m_controllerValue;
}
    
void SynthVoice::SetSampleRate(const double sampleRate)
{
	m_oscillator.SetSampleRate(sampleRate);
	m_envelope.SetSampleRate(sampleRate);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	for (int sampleIndex = startSample; sampleIndex < numSamples; sampleIndex++)
	{
		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			double sample = m_oscillator.OscCycleWithFreq(m_frequency, 1);
			sample = m_envelope.Process(sample);

			outputBuffer.addSample(channel, sampleIndex, sample);
		}
	}
}
