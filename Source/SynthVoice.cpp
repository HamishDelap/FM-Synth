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
	m_level = 1; velocity * 0.15;

	m_modulatorEnvelope.SetParameters({ 0.05, 0.05, 0.7, 0.4 });
	m_modulatorEnvelope.NoteOn();

	m_carrierEnvelope.SetParameters({ 0.05, 0.05, 0.7, 0.4 });
	m_carrierEnvelope.NoteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	if (allowTailOff)
	{
		m_modulatorEnvelope.NoteOff();
		m_carrierEnvelope.NoteOff();
	}
	else
	{
		clearCurrentNote();
	}
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
	m_modulator.SetSampleRate(sampleRate);
	m_modulator.SetWaveForm(lldsp::oscillators::Waveform::Sin);

	m_carrier.SetSampleRate(sampleRate);
	m_carrier.SetWaveForm(lldsp::oscillators::Waveform::Sawtooth);

	m_modulatorEnvelope.SetSampleRate(sampleRate);
	m_carrierEnvelope.SetSampleRate(sampleRate);

	m_filter = std::make_unique<lldsp::dsp::BiQuadFilter>(sampleRate);
	m_filter->SetCutoff(200, 1.0, 1);

	m_reverb = std::make_unique<lldsp::effects::Reverb>(sampleRate);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	for (int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
	{
		double modulatorOutput = m_modulator.Sample(m_frequency / 2, 0.1);
		modulatorOutput = m_modulatorEnvelope.Process(modulatorOutput);
		double sample = m_carrier.Sample(m_frequency + modulatorOutput, 1);
		sample = m_carrierEnvelope.Process(sample);
		m_filter->Process(sample);
		m_reverb->Process(sample, 4.0, 1.0);

		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			outputBuffer.addSample(channel, startSample, sample);
		}
		
		if (!m_carrierEnvelope.IsActive())
		{
			clearCurrentNote();
		}

		startSample++;
	}
}
