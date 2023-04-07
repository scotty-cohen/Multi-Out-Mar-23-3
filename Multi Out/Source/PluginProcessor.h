/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Week3SineGeneratorAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Week3SineGeneratorAudioProcessor();
    ~Week3SineGeneratorAudioProcessor() override;

    
    
    
    
    //==============================================================================
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;

    /* */
    
    void setGain (float inGain);
    
    /* */
    
    float getGain ();
    
    /* */
    
    
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

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
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
    //make a gain parameter
    juce::AudioParameterFloat* mGainParam;
    
    
    //make that gain (pan knob)smoothie
    juce::LinearSmoothedValue<float> mSmoothedGain;
    
    float phase = 0;
    
    
    //smooth the levels
    juce::LinearSmoothedValue<float> level1;
    juce::LinearSmoothedValue<float> level2;
    juce::LinearSmoothedValue<float> level3;
    juce::LinearSmoothedValue<float> level4;




    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Week3SineGeneratorAudioProcessor)
};
