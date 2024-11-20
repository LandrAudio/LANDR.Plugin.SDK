#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , processorRef(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 340);
    setResizable(false, false);

    addAndMakeVisible(m_title);
    juce::Font titleFont = m_title.getFont();
    titleFont.setHeight(30.0f);
    m_title.setFont(titleFont);
    m_title.setText("LANDR SDK Example", juce::dontSendNotification);
    m_title.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(m_subtitle);
    m_subtitle.setText("Enter a license key below", juce::dontSendNotification);
    m_subtitle.setJustificationType(juce::Justification::left);

    addAndMakeVisible(m_licenseKeyTextEditor);
    m_licenseKeyTextEditor.onTextChange = [this]() {
        m_activateLicenseBtn.setEnabled(m_licenseKeyTextEditor.getText() != "");
    };
    m_licenseKeyTextEditor.onReturnKey = [this]() { activateLicense(); };

    addAndMakeVisible(m_pasteBtn);
    m_pasteBtn.setButtonText("Paste");
    m_pasteBtn.onClick = [this]() {
        juce::String cleanedString = juce::SystemClipboard::getTextFromClipboard().trim().removeCharacters("\n\t");
        m_licenseKeyTextEditor.setText(cleanedString);
    };

    addAndMakeVisible(m_activateLicenseBtn);
    m_activateLicenseBtn.setButtonText("Activate");
    m_activateLicenseBtn.setEnabled(false);
    m_activateLicenseBtn.onClick = [this]() { activateLicense(); };

    addAndMakeVisible(m_status);
    m_status.setText("Status: ", juce::dontSendNotification);
    m_status.setJustificationType(juce::Justification::left);

    addChildComponent(m_subStatus);
    m_subStatus.setText("", juce::dontSendNotification);
    m_subStatus.setJustificationType(juce::Justification::left);

    addAndMakeVisible(m_gainLabel);
    m_gainLabel.setText("Gain", juce::dontSendNotification);
    m_gainLabel.setJustificationType(juce::Justification::left);

    addAndMakeVisible(m_gainSlider);
    m_gainSliderAttachment = std::make_unique<SliderAttachment>(processorRef.getParameterTreeState(), "gain", m_gainSlider);

    addAndMakeVisible(m_processingAudioTitle);
    m_processingAudioTitle.setText("Audio Processing Status:", juce::dontSendNotification);
    m_processingAudioTitle.setJustificationType(juce::Justification::left);

    addAndMakeVisible(m_processingAudioStatus);
    m_processingAudioStatus.setText("Unknown", juce::dontSendNotification);
    m_processingAudioStatus.setJustificationType(juce::Justification::left);

    addAndMakeVisible(m_sdkVersion);
    m_sdkVersion.setText("LANDR SDK Version: " + processorRef.getLicenser().getVersion(), juce::dontSendNotification);
    m_sdkVersion.setJustificationType(juce::Justification::left);

    updateUI();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(20, 20);
    m_title.setBounds(bounds.removeFromTop(40));
    m_subtitle.setBounds(bounds.removeFromTop(40));

    auto licenseKeyBounds = bounds.removeFromTop(40);
    m_licenseKeyTextEditor.setBounds(licenseKeyBounds.removeFromLeft(450));
    licenseKeyBounds.removeFromLeft(20);
    m_pasteBtn.setBounds(licenseKeyBounds.removeFromLeft(80));
    licenseKeyBounds.removeFromLeft(20);
    m_activateLicenseBtn.setBounds(licenseKeyBounds);

    m_status.setBounds(bounds.removeFromTop(40));
    m_subStatus.setBounds(bounds.removeFromTop(40));

    auto gainSliderBounds = bounds.removeFromTop(40);
    m_gainLabel.setBounds(gainSliderBounds.removeFromLeft(60));
    gainSliderBounds.removeFromLeft(50);
    m_gainSlider.setBounds(gainSliderBounds);

    auto audioProcessingBounds = bounds.removeFromTop(40);
    m_processingAudioTitle.setBounds(audioProcessingBounds.removeFromLeft(150));
    audioProcessingBounds.removeFromLeft(5);
    m_processingAudioStatus.setBounds(audioProcessingBounds);

    m_sdkVersion.setBounds(bounds);
}

void AudioPluginAudioProcessorEditor::activateLicense()
{
    processorRef.tryActivateWithKey(m_licenseKeyTextEditor.getText().toStdString());
    updateUI();
}

void AudioPluginAudioProcessorEditor::updateUI()
{
    if (!processorRef.checkLicense()) {
        auto const status = processorRef.getLicenser().getStatusInfo();
        const std::string msg = "Error (" + status.errorSubCode + ") : License not valid";
        m_status.setText(msg, juce::dontSendNotification);

        if (status.message != "") {
            m_subStatus.setText(status.message, juce::dontSendNotification);
            m_subStatus.setVisible(true);
        } else {
            m_subStatus.setVisible(false);
        }
        m_processingAudioStatus.setColour(juce::Label::textColourId, juce::Colours::red);
        m_processingAudioStatus.setText("Disabled", juce::dontSendNotification);
    } else {
        m_status.setText("Success: License is active", juce::dontSendNotification);
        m_subStatus.setVisible(false);
        m_processingAudioStatus.setColour(juce::Label::textColourId, juce::Colours::green);
        m_processingAudioStatus.setText("Enabled", juce::dontSendNotification);
    }
}
