#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

class JStateManager
{
    public:
        JStateManager(juce::AudioProcessor& audioProcessor);

        void writeState(juce::MemoryBlock&);
        void readState(const void*, int);

        void writePreset(const juce::String& presetName);
        void readPreset(const juce::String& presetName);

        static juce::StringArray getPresets();
        static juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

        juce::AudioProcessorValueTreeState apvt; // Could be std::unique_pointer?

    private:
        static std::optional<juce::File> getPresetDirectory()
		{
            const juce::String presetFolderString(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getFullPathName() + juce::File::getSeparatorString() + "MODULA");
            const juce::File presetFolder(presetFolderString);
            if (presetFolder.isDirectory() == false)
            {
                if (auto result = presetFolder.createDirectory(); result.failed())
                {
                    return std::nullopt;
                }
            }
            return(presetFolder);
		}
};