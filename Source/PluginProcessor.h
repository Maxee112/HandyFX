/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AubioWrapper.h"

//==============================================================================
/**
*/
class HandyFXAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    HandyFXAudioProcessor();
    ~HandyFXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
 
    //==============================================================================
    // Delay functions
    void HandyFXAudioProcessor::readCircularBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel);
    void HandyFXAudioProcessor::fillCircularBuffer(juce::AudioBuffer<float>& buffer, int channel);
    void HandyFXAudioProcessor::updateWritePosition(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer);
    //==============================================================================
    //==============================================================================
    // 
    juce::AudioProcessorValueTreeState parameters{ *this, nullptr, "Parameters", createParameterLayout() };

    float getBPM();

private:
    int writePosition = { 0 };
    juce::AudioBuffer<float> delayBuffer;
    AubioWrapper aubioWrapper;
    //==============================================================================
    juce::SmoothedValue<float> feedbackSmoothed;
    juce::SmoothedValue<float> delaySmoothed;
    //==============================================================================
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;
    juce::dsp::Chorus<float> vibrato;
    //==============================================================================
    float getDelayTimeFraction(int index);
    void setReverbParams();
    void setVibratoParams();
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    std::unique_ptr<juce::AudioProcessorParameterGroup> createDelayParameterGroup();
    std::unique_ptr<juce::AudioProcessorParameterGroup> createReverbParameterGroup();
    std::unique_ptr<juce::AudioProcessorParameterGroup> createVibratoParameterGroup();
    //==============================================================================
    void applyChosenEffect(juce::AudioBuffer<float>& buffer, int inputChannels);
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HandyFXAudioProcessor)
};
