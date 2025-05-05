#include "helpers/test_helpers.h"
#include "PluginProcessor.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE ("one is equal to one", "[dummy]")
{
    REQUIRE (1 == 1);
}

TEST_CASE ("Plugin instance", "[instance]")
{
    FMSynthAudioProcessor testPlugin;

    SECTION ("name")
    {
        CHECK_THAT (testPlugin.getName().toStdString(),
            Catch::Matchers::Equals ("Modula"));
    }
}

TEST_CASE("Empty input", "[instance]")
{
    FMSynthAudioProcessor processor;
    processor.prepareToPlay(44100.0, 512); // Typical setup

    // Create dummy buffers
    juce::AudioBuffer<float> audioBuffer(2, 512); // stereo buffer, 512 samples
    audioBuffer.clear();

    juce::MidiBuffer midiBuffer;

    processor.processBlock(audioBuffer, midiBuffer);
   
    float sample = audioBuffer.getSample(0, 0);
    REQUIRE(juce::approximatelyEqual(0.0f, sample));
}

#ifdef PAMPLEJUCE_IPP
    #include <ipp.h>

TEST_CASE ("IPP version", "[ipp]")
{
    CHECK_THAT (ippsGetLibVersion()->Version, Catch::Matchers::Equals ("2022.0.0 (r0x131e93b0)"));
}
#endif
