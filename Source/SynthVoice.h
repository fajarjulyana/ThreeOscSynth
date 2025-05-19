// ========================
// SynthVoice.h
// ========================
#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override {
        osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
        adsr.noteOn();
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override {
        adsr.noteOff();
        if (!allowTailOff || !adsr.isActive())
            clearCurrentNote();
    }

    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        if (!isVoiceActive()) return;

        synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
        synthBuffer.clear();

        juce::dsp::AudioBlock<float> audioBlock(synthBuffer);
        osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        adsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);

        for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
            outputBuffer.addFrom(ch, startSample, synthBuffer, ch, 0, numSamples);
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = outputChannels;

        osc.prepare(spec);
        osc.initialise([](float x) { return std::sin(x); });
        adsr.setSampleRate(sampleRate);

        juce::ADSR::Parameters adsrParams { 0.01f, 0.1f, 0.8f, 0.5f };
        adsr.setParameters(adsrParams);
    }

    void updateWaveform(int waveformIndex) {
        switch (waveformIndex)
        {
            case 0:
                osc.initialise([](float x) { return std::sin(x); }); break;
            case 1:
                osc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }); break;
            case 2:
                osc.initialise([](float x) { return std::asin(std::sin(x)) * (2.0f / juce::MathConstants<float>::pi); }); break;
            default:
                jassertfalse; break;
        }
    }

private:
    juce::dsp::Oscillator<float> osc;
    juce::ADSR adsr;
    juce::AudioBuffer<float> synthBuffer;
};
