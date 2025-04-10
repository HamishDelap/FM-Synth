#include "JStateManager.h"

JStateManager::JStateManager(AudioProcessor& audioProcessor) : 
    apvt(audioProcessor, nullptr, "DEFAULT", JStateManager::getParameterLayout())
{
    apvt.state = ValueTree("PRESETS");
    presetDirectory = getPresetDirectory();
}

void JStateManager::writeState(juce::MemoryBlock& destData) 
{
    // Copy the state to new variable
    auto state = apvt.copyState();
    // Create XML object
    std::unique_ptr<XmlElement> xml(state.createXml());

    // Write to binary just so readState is called on startup.
    AudioProcessor::copyXmlToBinary(*xml, destData);
}

// Read state from memory
void JStateManager::readState(const void* data, int sizeInBytes) 
{
    // Get XML document
    XmlDocument myDocument(File(presetDirectory.getFullPathName() + File::getSeparatorString() + "preset.xml"));
    // Parse the document
    std::unique_ptr<XmlElement> xmlState(myDocument.getDocumentElement());
    
    // If it is not a nullptr (parsing succeeded) then continue, otherwise get next value
    if (xmlState != nullptr) { apvt.state = ValueTree::fromXml(*xmlState); }

    JStateManager::getPresets();
}

void JStateManager::readPreset(String presetName) 
{
    XmlDocument myDocument(File(presetDirectory.getFullPathName() + File::getSeparatorString() + presetName));
    std::unique_ptr<XmlElement> xmlState(myDocument.getDocumentElement());;
    DBG(myDocument.getLastParseError());
    if (xmlState != nullptr) { apvt.state = ValueTree::fromXml(*xmlState); }
}

void JStateManager::writePreset(String presetName) 
{
    // Copy the state to new variable
    auto state = apvt.copyState();
    // Create XML object
    std::unique_ptr<XmlElement> xml(state.createXml());
	// Write XML to file
    xml->writeTo(File(presetDirectory.getFullPathName() + File::getSeparatorString() + presetName));
}

StringArray JStateManager::getPresets() 
{
    filenames.clear();
	
    for (DirectoryEntry entry : RangedDirectoryIterator(presetDirectory, true)) {
		filenames.add(entry.getFile().getFileName());
    }
	return(filenames);
}

AudioProcessorValueTreeState::ParameterLayout JStateManager::getParameterLayout()
{
        AudioProcessorValueTreeState::ParameterLayout params;
        // Modulator
        NormalisableRange<float> modRatioRange(0.5f, 10.0f, 0.25f);  // 0.1f is the step increment
        params.add(std::make_unique<AudioParameterFloat>("MOD_RATIO", "Modulator Ratio", modRatioRange, 2));
        params.add(std::make_unique<AudioParameterFloat>("MOD_LEVEL", "Modulator Level", 0, 100, 1.0));

        // NOT CURRENTLY IN USE
        params.add(std::make_unique<AudioParameterFloat>("MOD_ATTACK", "Modulator Attack", 0, 10, 0.5));
        params.add(std::make_unique<AudioParameterFloat>("MOD_DECAY", "Modulator Decay", 0, 10, 0.2));
        params.add(std::make_unique<AudioParameterFloat>("MOD_SUSTAIN", "Modulator Sustain", 0, 10, 1));
        params.add(std::make_unique<AudioParameterFloat>("MOD_RELEASE", "Modulator Release", 0, 10, 0.5));
        
        // Carrier
        params.add(std::make_unique<AudioParameterInt>("CARRIER_WAVEFORM", "Carrier Waveform", 1, 4, 4));
        params.add(std::make_unique<AudioParameterFloat>("CARRIER_ATTACK", "Carrier Attack", 0, 10, 0.25));
        params.add(std::make_unique<AudioParameterFloat>("CARRIER_DECAY", "Carrier Decay", 0, 10, 0.2));
        params.add(std::make_unique<AudioParameterFloat>("CARRIER_SUSTAIN", "Carrier Sustain", 0, 10, 1));
        params.add(std::make_unique<AudioParameterFloat>("CARRIER_RELEASE", "Carrier Release", 0, 10, 0.5));

        // Filter
        params.add(std::make_unique<AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff", 10, 20000, 3000));
        params.add(std::make_unique<AudioParameterFloat>("FILTER_Q", "Filter Q", 0.5, 1.5, 0));
        params.add(std::make_unique<AudioParameterFloat>("FILTER_ATTACK", "Filter Attack", 0, 10, 0.5));
        params.add(std::make_unique<AudioParameterFloat>("FILTER_DECAY", "Filter Decay", 0, 10, 0.2));
        params.add(std::make_unique<AudioParameterFloat>("FILTER_SUSTAIN", "Filter Sustain", 0, 1, 0.8));
        params.add(std::make_unique<AudioParameterFloat>("FILTER_RELEASE", "Filter Release", 0, 10, 0.5));

        // Reverb
        params.add(std::make_unique<AudioParameterFloat>("REVERB_TIME", "Reverb Time", 0, 10, 1));
        params.add(std::make_unique<AudioParameterFloat>("REVERB_AMOUNT", "Reverb Amount", 0, 1, 0.1));

        // Distortion
        params.add(std::make_unique<AudioParameterFloat>("DISTORTION_GAIN", "Distortion Gain", 0, 1, 0.25));

        return params;
}