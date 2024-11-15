#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "landr/LANDR.h"

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor {
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    bool checkLicense();
    bool tryActivateWithKey(const std::string& key);

    LANDR& getLandr() { return m_landr; }
    juce::AudioProcessorValueTreeState& getParameterTreeState() { return m_parameters; }

private:
    //==============================================================================
    LANDR m_landr;

    juce::AudioProcessorValueTreeState m_parameters;
    std::atomic<float>* m_gainParameter = nullptr;
    std::atomic<bool> m_processAudio = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
