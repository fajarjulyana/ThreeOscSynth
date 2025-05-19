// ========================
// PluginProcessor.cpp
// ========================
#include "PluginProcessor.h"
#include "PluginEditor.h"

ThreeOscSynthAudioProcessor::ThreeOscSynthAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameters())
{
    for (int i = 0; i < 3; ++i)
        synth.addVoice(new SynthVoice());

    synth.clearSounds();
    synth.addSound(new SynthSound());
}

juce::AudioProcessorValueTreeState::ParameterLayout ThreeOscSynthAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    auto choices = juce::StringArray { "Sine", "Square", "Triangle" };

    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc1Type", "Oscillator 1", choices, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc2Type", "Oscillator 2", choices, 1));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc3Type", "Oscillator 3", choices, 2));

    return { params.begin(), params.end() };
}

void ThreeOscSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void ThreeOscSynthAudioProcessor::releaseResources() {}

void ThreeOscSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            int waveIndex = apvts.getRawParameterValue("osc" + juce::String(i + 1) + "Type")->load();
            voice->updateWaveform(waveIndex);
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* ThreeOscSynthAudioProcessor::createEditor() { return new ThreeOscSynthAudioProcessorEditor(*this); }
bool ThreeOscSynthAudioProcessor::hasEditor() const { return true; }

const juce::String ThreeOscSynthAudioProcessor::getName() const { return "ThreeOscSynth"; }
bool ThreeOscSynthAudioProcessor::acceptsMidi() const { return true; }
bool ThreeOscSynthAudioProcessor::producesMidi() const { return false; }
bool ThreeOscSynthAudioProcessor::isMidiEffect() const { return false; }
double ThreeOscSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int ThreeOscSynthAudioProcessor::getNumPrograms() { return 1; }
int ThreeOscSynthAudioProcessor::getCurrentProgram() { return 0; }
void ThreeOscSynthAudioProcessor::setCurrentProgram (int index) {}
const juce::String ThreeOscSynthAudioProcessor::getProgramName (int index) { return {}; }
void ThreeOscSynthAudioProcessor::changeProgramName (int index, const juce::String& newName) {}

void ThreeOscSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ThreeOscSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ThreeOscSynthAudioProcessor();
}
