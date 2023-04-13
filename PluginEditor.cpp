
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PlugInGUISettings.h"

#include <vector>
//==============================================================================
#if WITH_MIDIKEYBOARD   
MatPlotTestAudioProcessorEditor::MatPlotTestAudioProcessorEditor (MatPlotTestAudioProcessor& p)
    : AudioProcessorEditor (&p), m_processorRef (p), m_presetGUI(p.m_presets),
    	m_keyboard(m_processorRef.m_keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
#else
MatPlotTestAudioProcessorEditor::MatPlotTestAudioProcessorEditor (MatPlotTestAudioProcessor& p)
    : AudioProcessorEditor (&p), m_processorRef (p), m_presetGUI(p.m_presets)
#endif
{
    float scaleFactor = m_processorRef.getScaleFactor();
    setResizeLimits (g_minGuiSize_x,g_minGuiSize_x*g_guiratio , g_maxGuiSize_x, g_maxGuiSize_x*g_guiratio);
    setResizable(true,true);
    getConstrainer()->setFixedAspectRatio(1./g_guiratio);
    setSize (scaleFactor*g_minGuiSize_x, scaleFactor*g_minGuiSize_x*g_guiratio);

	addAndMakeVisible(m_presetGUI);
#if WITH_MIDIKEYBOARD      
	addAndMakeVisible(m_keyboard);
#endif
    addAndMakeVisible(m_plotter);
}

MatPlotTestAudioProcessorEditor::~MatPlotTestAudioProcessorEditor()
{
}

//==============================================================================
void MatPlotTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    std::vector<std::vector<float>> x;
    x.resize(1);
    x[0].resize(8);
    x[0][0] = 1.f;
    x[0][1] = -0.5f;
    x[0][7] = 3.f;
    m_plotter.plot(x);
}
const int g_minPresetHandlerHeight(30);
const float g_midikeyboardratio(0.13);
void MatPlotTestAudioProcessorEditor::resized()
{
    int height = getHeight();
    // necessary to change fontsize of comboboxes and PopUpmenus
    // 0.5 is a good compromisecould be slightly higher or lower
    m_jadeLAF.setFontSize(0.5*height*g_minPresetHandlerHeight/g_minGuiSize_y);
    // top presethandler
    m_presetGUI.setBounds(0, 0, getWidth(), height*g_minPresetHandlerHeight/g_minGuiSize_y);
    // bottom a small midkeyboard
#if WITH_MIDIKEYBOARD    
    m_keyboard.setBounds(0, height*(1-g_midikeyboardratio), getWidth(), height*g_midikeyboardratio);
#endif
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    int width = getWidth();
	float scaleFactor = float(width)/g_minGuiSize_x;
    m_processorRef.setScaleFactor(scaleFactor);
    // use setBounds with scaleFactor
    auto bounds = getBounds();
    bounds.removeFromTop(100);
    m_plotter.setBounds(bounds);
}
