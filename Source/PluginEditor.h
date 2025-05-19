// ========================
// PluginEditor.h
// ========================
#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ThreeOscSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ThreeOscSynthAudioProcessorEditor (ThreeOscSynthAudioProcessor&);
    ~ThreeOscSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ThreeOscSynthAudioProcessor& audioProcessor;

    juce::ComboBox osc1Selector, osc2Selector, osc3Selector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> osc1Attachment, osc2Attachment, osc3Attachment;

    juce::MidiKeyboardState& keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreeOscSynthAudioProcessorEditor)
};

