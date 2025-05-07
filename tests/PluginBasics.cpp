#include "helpers/test_helpers.h"
#include "PluginProcessor.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

// Test static plugin config
TEST_CASE ("Plugin instance", "[instance]")
{
    SECTION ("name")
    {
        FMSynthAudioProcessor testPlugin;
        CHECK_THAT (testPlugin.getName().toStdString(),
            Catch::Matchers::Equals ("Modula"));
    }
}

// Test empty buffer output
TEST_CASE("Empty input", "[instance]")
{
    FMSynthAudioProcessor processor;
    processor.prepareToPlay(44100.0, 512); // Typical setup

    // Create dummy buffers
    juce::AudioBuffer<float> audioBuffer(2, 512); // stereo buffer, 512 samples
    audioBuffer.clear();

    juce::MidiBuffer midiBuffer;

    processor.processBlock(audioBuffer, midiBuffer);

    REQUIRE(IsBufferZeroed(audioBuffer));
}

TEST_CASE("Midi On", "[instance]")
{
    FMSynthAudioProcessor processor;
    processor.prepareToPlay(44100.0, 512); // Typical setup

    // Create dummy buffers
    juce::AudioBuffer<float> audioBuffer(2, 512); // stereo buffer, 512 samples
    audioBuffer.clear();

    juce::MidiBuffer midiBuffer;
    // Create a MIDI note-on message: channel 1, note 60 (Middle C), velocity 100
    juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, (juce::uint8)100);
    midiBuffer.addEvent(noteOn, 0);

    processor.processBlock(audioBuffer, midiBuffer);

    REQUIRE(!IsBufferZeroed(audioBuffer));
}

#ifdef PAMPLEJUCE_IPP
    #include <ipp.h>

TEST_CASE ("IPP version", "[ipp]")
{
    CHECK_THAT (ippsGetLibVersion()->Version, Catch::Matchers::Equals ("2022.0.0 (r0x131e93b0)"));
}
#endif
