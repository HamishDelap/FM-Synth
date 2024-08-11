#ifndef _JSTATEMANAGER_
#define _JSTATEMANAGER_

#include <JuceHeader.h>

using namespace juce;
class JStateManager
{
    public:
        JStateManager(AudioProcessor& audioProcessor);

        void writeState(MemoryBlock&);
        void readState(const void*, int);

        void writePreset(String presetName);
        void readPreset(String presetName);

        AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

        StringArray getPresets();
        AudioProcessorValueTreeState apvt; // Could be std::unique_pointer?
    private:
        static File getPresetDirectory()
		{
            const String presetFolderString(File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName() + File::getSeparatorString() + "FMATOR");
            const File presetFolder(presetFolderString);
            if (presetFolder.isDirectory() == false)
            {
                bool b = presetFolder.createDirectory();
            }
            return(presetFolder);
		}

        File presetDirectory;
        StringArray filenames;
};

#endif