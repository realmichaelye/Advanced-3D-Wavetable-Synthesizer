#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//#include "SynthSound.h" //(included in SynthVoice)
#include "SynthVoice.h"
//==============================================================================
/**
*/
class ArcAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ArcAudioProcessor();
    ~ArcAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	SynthSound* mSynthSound;

private:
	AudioProcessorValueTreeState mVTS;
	AudioParameterFloat* mParamMaster;

	AudioParameterFloat* getParamFloat(String id) const {
		return dynamic_cast<AudioParameterFloat*>(mVTS.getParameter(id));
	}
	AudioParameterInt* getParamInt(String id) const {
		return dynamic_cast<AudioParameterInt*>(mVTS.getParameter(id));
	}

	Synthesiser mSynth;

	dsp::Oversampling<float> mOversampling;

	//TODO: use atomic?
	double mLastSampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArcAudioProcessor)
};
