#pragma once
#include <PluginProcessor.h>

/* This is a helper function to run tests within the context of a plugin editor.
 *
 * Read more here: https://github.com/sudara/pamplejuce/issues/18#issuecomment-1425836807
 *
 * Example usage (screenshots the plugin)
 *
  runWithinPluginEditor ([&] (PluginProcessor& plugin) {
    auto snapshot = plugin.getActiveEditor()->createComponentSnapshot (plugin.getActiveEditor()->getLocalBounds(), true, 2.0f);
    auto file = juce::File::getSpecialLocation (juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile ("snapshot.jpeg");
    file.deleteFile();
    juce::FileOutputStream stream (file);
    juce::JPEGImageFormat jpeg;
    jpeg.writeImageToStream (snapshot, stream);

    REQUIRE (file.existsAsFile());
   });

 */
[[maybe_unused]] static void runWithinPluginEditor (const std::function<void (FMSynthAudioProcessor& plugin)>& testCode)
{
    FMSynthAudioProcessor plugin;
    const auto editor = plugin.createEditorIfNeeded();

    testCode(plugin);

    plugin.editorBeingDeleted (editor);
    delete editor;
}

[[maybe_unused]] static bool IsBufferZeroed(juce::AudioBuffer<float>& buffer)
{
    int size = buffer.getNumSamples();
    int channelCount = buffer.getNumChannels();

    for (int i = 0; i < channelCount; i++)
    {
        auto bufferPtr = buffer.getReadPointer(i);
        for (int j = 0; j < size; j++)
        {
            if (!juce::approximatelyEqual(bufferPtr[j], 0.0f))
            {
                return false;
            }
        }
    }
    return true;
}
