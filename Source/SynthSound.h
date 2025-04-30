#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNumber*/)
    {
        return true;
    }

    bool appliesToChannel(int /*midiChannel*/)
    {
        return true;
    }
};
