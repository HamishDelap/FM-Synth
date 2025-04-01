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
	m_trueFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	UpdateFrequency();
	m_level = 1; velocity * 0.15;

	m_modulatorEnvelope.SetParameters(m_parameters.modADSRParams);
	m_carrierEnvelope.SetParameters(m_parameters.carrierADSRParams);
	m_filterEnvelope.SetParameters(m_parameters.filterADSRParams);

	m_noteOn = true;

	m_modulatorEnvelope.NoteOn();
	m_carrierEnvelope.NoteOn();
	m_filterEnvelope.NoteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	m_noteOn = false;
	if (allowTailOff)
	{
		m_modulatorEnvelope.NoteOff();
		m_carrierEnvelope.NoteOff();
		m_filterEnvelope.NoteOff();
	}
	else
	{
		clearCurrentNote();
	}
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
	// Let's assume a pitch bend range of ±2 semitones for this example
	auto pitchBendRange = 2.0f; // in semitones

	// Normalize the pitch wheel value to a range of -1.0 to 1.0
	auto pitchBend = (newPitchWheelValue - 8192) / 8192.0f;

	// Calculate the pitch change in semitones
	m_pitchBendChange = pitchBendRange * pitchBend;
	if (m_noteOn)
	{
		UpdateFrequency();
	}
}
    
void SynthVoice::UpdateFrequency()
{
	// Update the frequency of the oscillator based on the pitch change
	m_newFrequency = m_trueFrequency * std::pow(2.0f, m_pitchBendChange / 12.0f);
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

	m_modulatorEnvelope.SetSampleRate(sampleRate);
	m_carrierEnvelope.SetSampleRate(sampleRate);
	m_filterEnvelope.SetSampleRate(sampleRate);

	m_filter.SetSampleRate(sampleRate);
}

void SynthVoice::SetParameters(const Parameters& parameters)
{
	m_parameters = parameters;
	m_modulatorEnvelope.SetParameters(m_parameters.modADSRParams);
	m_carrierEnvelope.SetParameters(m_parameters.carrierADSRParams);
	m_filterEnvelope.SetParameters(m_parameters.filterADSRParams);
	m_carrier.SetWaveForm(m_parameters.carrierWaveform);
	m_modulator.SetWaveForm(m_parameters.carrierWaveform);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	// During processing
	for (int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++)
	{
		double modulatorOutput = m_modulator.Sample((m_newFrequency / m_parameters.modRatio), 1);
		//modulatorOutput = m_modulatorEnvelope.Process(modulatorOutput);
		double sample = m_carrier.Sample(std::abs(m_newFrequency * (modulatorOutput * m_parameters.modLevel)), 1);
		sample = m_carrierEnvelope.Process(sample); 

		if (m_parameters.filterCutoff != 0)
		{
			m_filter.SetCutoff(m_filterEnvelope.Process(m_parameters.filterCutoff), m_parameters.filterQ, 1);
			sample = m_filter.Process(sample);
		}

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
