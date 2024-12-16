#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor {
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void activateLicense();
    void resetLicense();
    void updateUI();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    juce::Label m_title;
    juce::Label m_subtitle;

    juce::TextEditor m_licenseKeyTextEditor;
    juce::TextButton m_activateLicenseBtn;
    juce::TextButton m_resetLicenseBtn;
    juce::TextButton m_pasteBtn;

    juce::Label m_status;
    juce::Label m_subStatus;

    juce::Label m_gainLabel;
    juce::Slider m_gainSlider;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> m_gainSliderAttachment;

    juce::Label m_processingAudioTitle;
    juce::Label m_processingAudioStatus;

    juce::Label m_sdkVersion;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
