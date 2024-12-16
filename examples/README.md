# LANDR SDK Examples

There are two simple examples demonstrating the useage of the LANDR SDK
- A CLI application which displays information about a license
- A simple [JUCE](https://juce.com/) based audio plugin 

## CLI application

### Usage

Simply run the following from the command line

```
cd /path/to/built/application
./licenser <license key>
```

This will return information about the license supplied with the `<license key>` parameter.

```
License Status:
-----------------
         Title: License Inactive
       Message: Your license is no longer active.
                Please resubscribe or purchase the plugin, and enter your new license key.
     Link Text: View pricing
      Link URL: https://www.landr.com/pricing
 Error Subcode: C53
```

## JUCE Plugin
### Installation

Simply copy the plugin into the relevant system plugin directory

#### macOS

- VST3: `/Library/Audio/Plug-Ins/VST3`
- AU: `/Library/Audio/Plug-Ins/Components`
- AAX: `/Library/Application\ Support/Avid/Audio/Plug-Ins`

#### Windows

- VST3: `C:\Program Files\Common Files\VST3`
- AAX: `C:\Program Files\Common Files\Avid\Audio\Plug-Ins`

### Usage

There are two states the plugin can be in depending on the validity of the license: 
- License Valid: In this state audio processing is enabled
- License not valid: In this state audio processing is disabled

In a DAW of your choice load the plugin onto an audio track. You will be presented with an input field to enter a product license key. Clicking submit will attempt to validate the license & update the state accordingly.

In terms of audio processing the plugin has a single slider to modify the gain of incoming audio, it does no audio generation of its own, so to test the processing its necessary to put an audio clip on the track. If the license is not valid no audio will pass through the plugin and the outputs will be zeroed. If the license is valid the input audio will flow through the processing and the volume will be modified by the gain parameter. 
