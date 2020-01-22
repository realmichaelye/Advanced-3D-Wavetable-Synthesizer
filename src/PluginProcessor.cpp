#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamIDs.h"
//#define newParamSkewed(id, min, max, defaultVal, skewFactor) (std::make_unique<AudioParameterFloat> (id, id, NormalisableRange<float>(min, max, 0.0f, skewFactor, false), defaultVal))
#define newParam(id, min, max, defaultVal) (std::make_unique<AudioParameterFloat> (id, id, NormalisableRange<float>(min,max, 0.0f), defaultVal))
#define newParamInt(id, min, max, defaultVal) (std::make_unique<AudioParameterInt> (id, id, min,max, defaultVal))
#define newParamPercent(id, defaultVal) (std::make_unique<AudioParameterFloat> (id, id, NormalisableRange<float>(0.0f,1.0f, 0.0f), defaultVal))
#define newParamPercentSkewed(id, defaultVal, skewFactor) (std::make_unique<AudioParameterFloat> (id, id, NormalisableRange<float>(0.0f,1.0f, 0.0f, skewFactor), defaultVal))

#define OVERSAMPLE_FACTOR_EXP 1 //2 to the power of
#define OVERSAMPLE_FACTOR 2

#define INIT_SAMPLE_RATE 44100
//==============================================================================
ArcAudioProcessor::ArcAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	//TODO: Oversampling
	mOversampling(2, OVERSAMPLE_FACTOR_EXP, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, false),
	//TODO: ParamTree
	mVTS(*this, nullptr, Identifier("main"), {
		newParamPercent(ID_masterVol, 0.5f),
		//Osc1
		newParamInt(ID_osc1_voices_int, 1, VOICE_MAX_NUMBER, 1),
		//todo change default
		newParamPercent(ID_osc1_wave, 0.0f),//done
		newParamPercent(ID_osc1_phase, 0.0f),//done

		newParam(ID_osc1_pitch, -1.0f, 1.0f, 0.0f, 0.0f), //NEED FIXING
		newParam(ID_osc1_fine, -100.0f, 100.0f, 0.0f, 0.0f), //done

		newParamPercentSkewed(ID_osc1_detune, 0.25f, 0.5f),//done
		//newParamPercent(ID_osc1_detune, 0.5f),//done
		newParamPercent(ID_osc1_wide, 1.0f),//done

		newParam(ID_osc1_pan, -1.0f, 1.0f, 0.0f, 0.0f),
		newParamPercent(ID_osc1_volume, 0.8f)

		})
#endif
{
	mParamMaster = getParamFloat(ID_masterVol);
	//clearSynth
	mSynth.clearSounds();
	mSynthSound = new SynthSound(mVTS);
	mSynth.addSound(mSynthSound);
	mSynth.clearVoices();
	const float numVoices = 8;
	for (int i = 0; i < numVoices; i++) {
		mSynth.addVoice(new SynthVoice());
	}
}

ArcAudioProcessor::~ArcAudioProcessor()
{
	mSynthSound = nullptr;
	mParamMaster = nullptr;
}

//==============================================================================
const String ArcAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ArcAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ArcAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ArcAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ArcAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ArcAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ArcAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ArcAudioProcessor::setCurrentProgram (int index)
{
}

const String ArcAudioProcessor::getProgramName (int index)
{
    return {};
}

void ArcAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ArcAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	//ignoreUnused(samplesPerBlock);
	mLastSampleRate = sampleRate;
	mSynth.setCurrentPlaybackSampleRate(mLastSampleRate*OVERSAMPLE_FACTOR);
	
	//Oversampling
	mOversampling.initProcessing(static_cast<size_t>(samplesPerBlock));

}

void ArcAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArcAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void ArcAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

	//Oversampling
	dsp::AudioBlock<float> block(buffer);
	dsp::AudioBlock<float> oversampledBlock;

	setLatencySamples(roundToInt(mOversampling.getLatencyInSamples()));
	oversampledBlock = mOversampling.processSamplesUp(block);
	//conversion to audiobuffer

	//constexpr int maxNumChannels = 2;
	float* channels[2] = { oversampledBlock.getChannelPointer(0), oversampledBlock.getChannelPointer(1) };
	AudioBuffer<float> oversampledBuffer{ channels, 2,
		static_cast<int>(oversampledBlock.getNumSamples()) };
	//conversion
	mSynth.renderNextBlock(oversampledBuffer, midiMessages, 0, oversampledBuffer.getNumSamples());

	mOversampling.processSamplesDown(block);


	//compressor



	//filter/emulator

	//final gain
	buffer.applyGain(mParamMaster->get());
}

//==============================================================================
bool ArcAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ArcAudioProcessor::createEditor()
{
    return new ArcAudioProcessorEditor (*this, mVTS);
}

//==============================================================================
void ArcAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ArcAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ArcAudioProcessor();
}
