/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Week3SineGeneratorAudioProcessorEditor::Week3SineGeneratorAudioProcessorEditor (Week3SineGeneratorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //pointers to JUCE processes
    auto gain_param = static_cast <juce::AudioParameterFloat*> (audioProcessor.getParameters()[0]);
    auto gain_range = gain_param -> getNormalisableRange();
    
    //Set our Slider Style
    mGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mGain.setRange(gain_range.start, gain_range.end);
    mGain.setValue(gain_range.convertFrom0to1(gain_param -> get()));
    
    //Send the value changes to the gain_param
    mGain.onValueChange = [this, gain_param] () {
        
        gain_param -> setValueNotifyingHost(mGain.getValue());
        audioProcessor.setGain(mGain.getValue());
    };

    //Make it visible
    addAndMakeVisible(mGain);
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    
    
}

Week3SineGeneratorAudioProcessorEditor::~Week3SineGeneratorAudioProcessorEditor()
{
}

//==============================================================================
void Week3SineGeneratorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void Week3SineGeneratorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    mGain.setBounds (getLocalBounds());
    mGain.setTextBoxStyle(juce::Slider::TextBoxBelow, true, getWidth(), 20);
}

void Week3SineGeneratorAudioProcessorEditor::timerCallback()
{
    
}
