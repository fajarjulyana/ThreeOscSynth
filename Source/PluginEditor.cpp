
// ========================
// PluginEditor.cpp
// ========================
#include "PluginEditor.h"

ThreeOscSynthAudioProcessorEditor::ThreeOscSynthAudioProcessorEditor (ThreeOscSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      keyboardState(p.keyboardState),
      keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (400, 200);

    auto& params = audioProcessor.apvts;
    auto choices = juce::StringArray { "Sine", "Square", "Triangle" };

    osc1Selector.addItemList(choices, 1);
    osc2Selector.addItemList(choices, 1);
    osc3Selector.addItemList(choices, 1);

    addAndMakeVisible(osc1Selector);
    addAndMakeVisible(osc2Selector);
    addAndMakeVisible(osc3Selector);
    addAndMakeVisible(keyboardComponent);

    osc1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(params, "osc1Type", osc1Selector);
    osc2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(params, "osc2Type", osc2Selector);
    osc3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(params, "osc3Type", osc3Selector);
}

ThreeOscSynthAudioProcessorEditor::~ThreeOscSynthAudioProcessorEditor() {}

void ThreeOscSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Three Oscillator Synth", 10, 60, getWidth(), 20, juce::Justification::centred, 1);
}

void ThreeOscSynthAudioProcessorEditor::resized()
{
    osc1Selector.setBounds(10, 10, 100, 30);
    osc2Selector.setBounds(120, 10, 100, 30);
    osc3Selector.setBounds(230, 10, 100, 30);
    keyboardComponent.setBounds(10, getHeight() - 70, getWidth() - 20, 60);
}

