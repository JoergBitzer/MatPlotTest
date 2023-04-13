#pragma once

#include "PluginProcessor.h"
#include "JadeLookAndFeel.h"
#include "PresetHandler.h"

#include <cmp_plot.h>
//#include <include/include/cmp_plot.h>


//==============================================================================
class MatPlotTestAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    
    explicit MatPlotTestAudioProcessorEditor (MatPlotTestAudioProcessor&);
    ~MatPlotTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    cmp::Plot m_plotter;
    JadeLookAndFeel m_jadeLAF;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MatPlotTestAudioProcessor& m_processorRef;
    PresetComponent m_presetGUI;
#if WITH_MIDIKEYBOARD    
    MidiKeyboardComponent m_keyboard;
#endif
    // plugin specific components

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatPlotTestAudioProcessorEditor)
};
