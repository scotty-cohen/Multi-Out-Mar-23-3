/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
void Week3SineGeneratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    mSmoothedGain.reset(sampleRate, 0.01);
    mSmoothedGain.setCurrentAndTargetValue(0.5f);
    
   
}


void Week3SineGeneratorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    /* boiler plate stuff to not touch */
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    //Set up parameters
    DBG ("GAIN: " << mGainParam -> get());
    

    //create buses
    auto busBuffer1 = getBusBuffer(buffer, false, 1);
    auto busBuffer2 = getBusBuffer(buffer, false, 2);
    auto busBuffer3 = getBusBuffer(buffer, false, 3);
    auto busBuffer4 = getBusBuffer(buffer, false, 4);
    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        
        
        // create value of position along knob to create levels
        double panPosition = mSmoothedGain.getNextValue();
        
        
        
        
        auto left_sample = buffer.getSample(0, sample);
        auto right_sample = buffer.getSample(1, sample);
        
        
        //linear panning algorythm for gain knob
        
        //number of busses
        const int numChannels = 4;
        
        
        //the width of a 'notch' in the slider
          auto notch = 1.0/numChannels;
          
          //home of position of each bus on the slider
           auto homePosition1 = notch * 0;
           auto homePosition2 = notch * 1;
           auto homePosition3 = notch * 2;
           auto homePosition4 = notch * 3;
          
          //normalized slider position (0 - 1)
           auto sliderPosition = panPosition;
           //sliderPosition = panPosition/panPosition.getMax;
          
        
          double level1 = abs(homePosition1 - sliderPosition);
          //level1 = constrain(level1, 0, notch);
          level1 = juce::jlimit(0.0, notch, level1);
          level1 = level1/notch;
          level1 = 1.0 - level1;
          
          double level2 = abs(homePosition2 - sliderPosition);
          level2 = juce::jlimit(0.0, notch, level2);
          level2 = level2/notch;
          level2 = 1.0 - level2;
          
          double level3 = abs(homePosition3 - sliderPosition);
          level3 = juce::jlimit(0.0, notch, level3);
          level3 = level3/notch;
          level3 = 1.0 - level3;
          
          double level4 = abs(homePosition4 - sliderPosition);
          level4 = juce::jlimit(0.0, notch, level4);
          level4 = level4/notch;
          level4 = 1.0 - level4;
        
        
        //pan audio buffer through busses
            busBuffer1.setSample(0, (sample*level1), left_sample);
            busBuffer1.setSample(1, (sample*level1), right_sample);
        
        
            busBuffer2.setSample(0, (sample*level2), left_sample);
            busBuffer2.setSample(1, (sample*level2), right_sample);
        
        
            busBuffer3.setSample(0, (sample*level3), left_sample);
            busBuffer3.setSample(1, (sample*level3), right_sample);
        
        
            busBuffer4.setSample(0, (sample*level4), left_sample);
            busBuffer4.setSample(1, (sample*level4), right_sample);
        
        std::cout << level1 << level2 << level3 << level4 << std::endl;

    }

    
    
}

/* */

void Week3SineGeneratorAudioProcessor::setGain (float inGain){
    
    mSmoothedGain.setTargetValue(inGain);
    
}

/* */

float Week3SineGeneratorAudioProcessor::getGain(){
    
    return mSmoothedGain.getCurrentValue();
}



//==============================================================================
Week3SineGeneratorAudioProcessor::Week3SineGeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
//This is where we added 4 extra output buses: "Name", Channel Config, Activated By Default
                       .withOutput ("Bus #1",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Bus #2",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Bus #3",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Bus #4",  juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    
    //gain parameter
    
    mGainParam = new juce::AudioParameterFloat ({"Gain", 1}, "Gain", 0.f, 1.f, .5f);
    addParameter (mGainParam);
}


Week3SineGeneratorAudioProcessor::~Week3SineGeneratorAudioProcessor()
{
}

//==============================================================================
const juce::String Week3SineGeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Week3SineGeneratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Week3SineGeneratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Week3SineGeneratorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Week3SineGeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Week3SineGeneratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Week3SineGeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Week3SineGeneratorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Week3SineGeneratorAudioProcessor::getProgramName (int index)
{
    return {};
}

void Week3SineGeneratorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void Week3SineGeneratorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Week3SineGeneratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
bool Week3SineGeneratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Week3SineGeneratorAudioProcessor::createEditor()
{
    return new Week3SineGeneratorAudioProcessorEditor (*this);
}

//==============================================================================
void Week3SineGeneratorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ValueTree tree("PARAMETER_STATE");
    tree.setProperty("GAIN", mGainParam -> get(), nullptr);
    std::unique_ptr<juce::XmlElement> xml(tree.createXml());
    
    copyXmlToBinary(*xml, destData);
}

void Week3SineGeneratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    
    juce::ValueTree tree = juce::ValueTree::fromXml(*xml);
    
    float gain = tree.getProperty("GAIN");
    mGainParam -> setValueNotifyingHost(gain);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Week3SineGeneratorAudioProcessor();
}
