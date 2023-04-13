#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MatPlotTestAudioProcessor::MatPlotTestAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{

    // It is important to have the paramter set in the m_paramVector
    // e.g. (a better solution is to ue this function in the components)
/*        m_paramVector.push_back(std::make_unique<AudioParameterFloat>(paramHpCutoff.ID,
        paramHpCutoff.name,
        NormalisableRange<float>(paramHpCutoff.minValue, paramHpCutoff.maxValue),
        paramHpCutoff.defaultValue,
        paramHpCutoff.unitName,
        AudioProcessorParameter::genericParameter,
        [](float value, int MaxLen) { value = int(exp(value) * 10) * 0.1;  return (String(value, MaxLen) + " Hz"); },
        [](const String& text) {return text.getFloatValue(); }));
//*/
    // this is just a placeholder (necessary for compiling/testing the template)
    m_paramVector.push_back(std::make_unique<AudioParameterFloat>("ExampeID",
        "Exampple Name",
        NormalisableRange<float>(1.f, 2.f),
        1.5f,
        "unitname",
        AudioProcessorParameter::genericParameter));
    
    m_parameterVTS = std::make_unique<AudioProcessorValueTreeState>(*this, nullptr, Identifier("MatPlotTestVTS"),
        AudioProcessorValueTreeState::ParameterLayout(m_paramVector.begin(), m_paramVector.end()));

	m_presets.setAudioValueTreeState(m_parameterVTS.get());
    // if needed add categories
    // m_presets.addCategory(StringArray("Unknown", "Soft", "Medium", "Hard", "Experimental"));
    // m_presets.addCategory(JadeSynthCategories);
#ifdef FACTORY_PRESETS    
    m_presets.DeployFactoryPresets();
#endif

	m_presets.loadfromFileAllUserPresets();    
}

MatPlotTestAudioProcessor::~MatPlotTestAudioProcessor()
{

}

//==============================================================================
const juce::String MatPlotTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MatPlotTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MatPlotTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MatPlotTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MatPlotTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MatPlotTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MatPlotTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MatPlotTestAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String MatPlotTestAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void MatPlotTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void MatPlotTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // sometimes you need the number of channels.
    // Since we only support if in and output is the same, we can just ask for input
    // 
    int nrofchannels = this->getMainBusNumOutputChannels();
    jassert(("number of channels should never be zero", nrofchannels>0));

    juce::ignoreUnused (samplesPerBlock);
    m_fs = sampleRate;
}

void MatPlotTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool MatPlotTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void MatPlotTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
 #if WITH_MIDIKEYBOARD  
	m_keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
#else
    juce::ignoreUnused (midiMessages);
#endif

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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        // ..do something to the data...
    }
#if WITH_MIDIKEYBOARD  
    midiMessages.clear(); // except you want to create new midi messages, but than say so 
    // by setting NEEDS_MIDI_OUTPUT in CMakeLists.txt
#endif
}

//==============================================================================
bool MatPlotTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MatPlotTestAudioProcessor::createEditor()
{
    return new MatPlotTestAudioProcessorEditor (*this);
}

//==============================================================================
void MatPlotTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	auto state = m_parameterVTS->copyState();
    ValueTree vtpluginsize("PluginSize");
    vtpluginsize.setProperty("ScaleFactor",m_pluginScaleFactor,nullptr);
    state.appendChild(vtpluginsize,nullptr);


	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);

}

void MatPlotTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
 	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(m_parameterVTS->state.getType()))
        {
            ValueTree vt = ValueTree::fromXml(*xmlState);
            ValueTree subvt = vt.getChildWithName("PluginSize");
            if (subvt.isValid())
            {
                float val = subvt.getProperty("ScaleFactor");
                m_pluginScaleFactor = val;
            }
			m_parameterVTS->replaceState(vt);
        }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MatPlotTestAudioProcessor();
}
