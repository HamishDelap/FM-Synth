#include "JStateManager.h"

JStateManager::JStateManager(juce::AudioProcessor& audioProcessor) :
    apvt(audioProcessor, nullptr, "DEFAULT", JStateManager::getParameterLayout())
{
    apvt.state = juce::ValueTree("PRESETS");
}

void JStateManager::writeState(juce::MemoryBlock& destData) 
{
    // Copy the state to new variable
    auto state = apvt.copyState();
    // Create XML object
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    // Write to binary just so readState is called on startup.
    juce::AudioProcessor::copyXmlToBinary(*xml, destData);
}

// Read state from memory
void JStateManager::readState(const void* data, int sizeInBytes) 
{
    std::unique_ptr<juce::XmlElement> xmlState = juce::AudioProcessor::getXmlFromBinary(data, sizeInBytes);
    if (xmlState != nullptr) { apvt.state = juce::ValueTree::fromXml(*xmlState); }
}

void JStateManager::readPreset(const juce::String& presetName)
{
    auto presetDirectory = getPresetDirectory();
    if (!presetDirectory)
    {
        // TODO: Handle invalid preset directory
        jassertfalse;
        return;
}
    juce::XmlDocument myDocument(juce::File(presetDirectory.value().getFullPathName() + juce::File::getSeparatorString() + presetName));
    std::unique_ptr<juce::XmlElement> xmlState(myDocument.getDocumentElement());;
    DBG(myDocument.getLastParseError());
    if (xmlState != nullptr) { apvt.state = juce::ValueTree::fromXml(*xmlState); }
}

void JStateManager::writePreset(const juce::String& presetName)
{
    auto presetDirectory = getPresetDirectory();
    if (!presetDirectory)
    {
        // TODO: Handle invalid preset directory
        jassertfalse;
        return;
    }
    // Copy the state to new variable
    auto state = apvt.copyState();
    // Create XML object
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
	// Write XML to file
    xml->writeTo(juce::File(presetDirectory.value().getFullPathName() + juce::File::getSeparatorString() + presetName));
}

juce::StringArray JStateManager::getPresets()
{
    auto presetDirectory = getPresetDirectory();
    if (!presetDirectory)
    {
        // TODO: Handle invalid preset directory
        jassertfalse;
        return {};
    }
    juce::StringArray presets;
    for (const juce::DirectoryEntry& entry : juce::RangedDirectoryIterator(presetDirectory.value(), true)) {
		presets.add(entry.getFile().getFileName());
    }
	return presets;
}

juce::AudioProcessorValueTreeState::ParameterLayout JStateManager::getParameterLayout()
{
        juce::AudioProcessorValueTreeState::ParameterLayout params;
        // Modulator
        juce::NormalisableRange<float> modRatioRange(0.5f, 10.0f, 0.25f);  // 0.1f is the step increment
        params.add(std::make_unique<juce::AudioParameterFloat>("MOD_RATIO", "Modulator Ratio", modRatioRange, 2.0f));
        params.add(std::make_unique<juce::AudioParameterFloat>("MOD_LEVEL", "Modulator Level", 0.0f, 100.0f, 1.0f));

        // NOT CURRENTLY IN USE
        params.add(std::make_unique<juce::AudioParameterFloat>("MOD_ATTACK", "Modulator Attack", 0.0f, 10.0f, 0.5f));
        params.add(std::make_unique<juce::AudioParameterFloat>("MOD_DECAY", "Modulator Decay", 0.0f, 10.0f, 0.2f));
        params.add(std::make_unique<juce::AudioParameterFloat>("MOD_SUSTAIN", "Modulator Sustain", 0.0f, 10.0f, 1.0f));
        params.add(std::make_unique<juce::AudioParameterFloat>("MOD_RELEASE", "Modulator Release", 0.0f, 10.0f, 0.5f));

        // Carrier
        params.add(std::make_unique<juce::AudioParameterInt>("CARRIER_WAVEFORM", "Carrier Waveform", 1, 4, 4));
        juce::NormalisableRange<float> carrierAttackRange{0.01f, 10.0f, 0.01f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("CARRIER_ATTACK", "Carrier Attack", carrierAttackRange, 0.25f));
        juce::NormalisableRange<float> carrierDecayRange{0.01f, 10.0f, 0.01f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("CARRIER_DECAY", "Carrier Decay", carrierDecayRange, 0.2f));
        params.add(std::make_unique<juce::AudioParameterFloat>("CARRIER_SUSTAIN", "Carrier Sustain", 0.0f, 1.0f, 1.0f));
        juce::NormalisableRange<float> carrierReleaseRange{0.01f, 10.0f, 0.01f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("CARRIER_RELEASE", "Carrier Release", carrierReleaseRange, 0.5f));

        // Filter
        juce::NormalisableRange<float> cutoffRange{10.0f, 20000.0f, 10.0f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff", cutoffRange, 3000.0f));
        params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_Q", "Filter Q", 0.5f, 1.5f, 0.0f));
        juce::NormalisableRange<float> filterAttackRange{0.01f, 10.0f, 0.01f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_ATTACK", "Filter Attack", filterAttackRange, 0.5f));
        juce::NormalisableRange<float> filterDecayRange{0.01f, 10.0f, 0.01f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_DECAY", "Filter Decay", filterDecayRange, 0.2f));
        params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_SUSTAIN", "Filter Sustain", 0.0f, 1.0f, 0.8f));
        juce::NormalisableRange<float> filterReleaseRange{0.01f, 10.0f, 0.01f, 0.6f};
        params.add(std::make_unique<juce::AudioParameterFloat>("FILTER_RELEASE", "Filter Release", filterReleaseRange, 0.5f));

        // Reverb
        params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_TIME", "Reverb Time", 0.0f, 10.0f, 1.0f));
        params.add(std::make_unique<juce::AudioParameterFloat>("REVERB_AMOUNT", "Reverb Amount", 0.0f, 1.0f, 0.1f));

        // Distortion
        params.add(std::make_unique<juce::AudioParameterFloat>("DISTORTION_GAIN", "Distortion Gain", 0.0f, 1.0f, 0.25f));
        return params;
}