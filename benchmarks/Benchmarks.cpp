#include "PluginEditor.h"
#include "catch2/benchmark/catch_benchmark_all.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE ("Boot performance")
{
    BENCHMARK_ADVANCED ("Processor constructor")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<Catch::Benchmark::storage_for<FMSynthAudioProcessor>> storage (size_t (meter.runs()));
        meter.measure ([&] (int i) { storage[(size_t) i].construct(); });
    };

    BENCHMARK_ADVANCED ("Processor destructor")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<Catch::Benchmark::destructable_object<FMSynthAudioProcessor>> storage (size_t (meter.runs()));
        for (auto& s : storage)
            s.construct();
        meter.measure ([&] (int i) { storage[(size_t) i].destruct(); });
    };

    BENCHMARK_ADVANCED ("Editor open and close")
    (Catch::Benchmark::Chronometer meter)
    {
        FMSynthAudioProcessor plugin;

        // due to complex construction logic of the editor, let's measure open/close together
        meter.measure ([&] (int /* i */) {
            auto editor = plugin.createEditorIfNeeded();
            plugin.editorBeingDeleted (editor);
            delete editor;
            return plugin.getActiveEditor();
        });
    };
    
    BENCHMARK_ADVANCED ("Empty processBlock")
    (Catch::Benchmark::Chronometer meter)
    {
        FMSynthAudioProcessor plugin;
		FMSynthAudioProcessor processor;
		processor.prepareToPlay(44100.0, 512); // Typical setup
		// Create dummy buffers
		juce::AudioBuffer<float> audioBuffer(2, 512); // stereo buffer, 512 samples
		audioBuffer.clear();
		juce::MidiBuffer midiBuffer;

        meter.measure ([&] (int /* i */) {
			processor.processBlock(audioBuffer, midiBuffer);
        });
    };
}
