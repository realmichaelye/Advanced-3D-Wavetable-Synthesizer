/*
  ==============================================================================

    SynthSound.h
    Created: 9 Aug 2019 7:44:15pm
    Author:  Michael

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Wavetable.h"
#include "ParamIDs.h"

#define PARAM_Float(id) dynamic_cast<AudioParameterFloat*>(vts.getParameter(id))
#define PARAM_Int(id) dynamic_cast<AudioParameterInt*>(vts.getParameter(id))
class SynthSound : public SynthesiserSound//, private AudioProcessorValueTreeState::Listener
{
public:
	SynthSound(AudioProcessorValueTreeState& vts) :
		mParamVoices(PARAM_Int(ID_osc1_voices_int)),

		waveTable(PARAM_Float(ID_osc1_wave)),
		mParamPhase(PARAM_Float(ID_osc1_phase)),

		mParamPitch(PARAM_Float(ID_osc1_pitch)),
		mParamFine(PARAM_Float(ID_osc1_fine)),

		mParamDetune(PARAM_Float(ID_osc1_detune)),
		mParamWide(PARAM_Float(ID_osc1_wide)),

		mParamPan(PARAM_Float(ID_osc1_pan)),
		mParamVolume(PARAM_Float(ID_osc1_volume))
	{
		
	}

	bool appliesToNote(int /*midiNoteNumber*/) noexcept {
		return true;
	}
	bool appliesToChannel(int /*midiChannel*/) noexcept {
		return true;
	}

	Array<float>* getCurrentWave() const {
		return waveTable.getCurrentWave();
	}

	AudioProcessorValueTreeState* vts;

	AudioParameterInt* mParamVoices;
	AudioParameterFloat* mParamPhase;

	AudioParameterFloat* mParamPitch;
	AudioParameterFloat* mParamFine;

	AudioParameterFloat* mParamDetune;
	AudioParameterFloat* mParamWide;

	AudioParameterFloat* mParamPan;
	AudioParameterFloat* mParamVolume;
	~SynthSound() {

		mParamVoices = nullptr;
		mParamPhase = nullptr;

		mParamPitch = nullptr;
		mParamFine = nullptr;
		
		mParamDetune = nullptr;
		mParamWide = nullptr;
		
		mParamPan = nullptr;
		mParamVolume = nullptr;
		
	}
	
	int RoundToMultiple(int toRound, int multiple) {
		toRound += multiple / 2;
		return toRound - (toRound%multiple);
	}

	void normalizeBuffer(AudioSampleBuffer& buffer) {
		////for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); ++sampleIndex) {
		////	DBG(buffer.getSample(0, sampleIndex));
		////}
		//Range<float> range = buffer.findMinMax(0, 0, buffer.getNumSamples());
		////DBG(range);
		//float min = range.getStart();
		//float max = range.getEnd();
		////float min = buffer->findMinMax(0, 0, buffer->getNumSamples()).;
		////float max = buffer->findMinMax(0, 0, buffer->getNumSamples()).getEnd();

		////DBG(min);
		////DBG(max);

		//for (int i = 0; i < buffer.getNumSamples(); ++i) {
		//	//DBG("current sample");
		//	//DBG(buffer->getSample(0, i));
		//	buffer.setSample(0, i, ((buffer.getSample(0, i) - min) /
		//		(max - min)) * 2 - 1);

		//}
		const float magnitude = buffer.getMagnitude(0, buffer.getNumSamples());
		jassert(magnitude > 0);
		buffer.applyGain(1.0f / magnitude);
		float avg = 0.0f;
		for (int i = 0; i < buffer.getNumSamples(); ++i) {
			avg += buffer.getSample(0, i);
		}
		avg /= buffer.getNumSamples();
		for (int i = 0; i < buffer.getNumSamples(); ++i) {
			buffer.setSample(0, i, buffer.getSample(0, i) - avg);
		//	buffer.setSample(0, i, buffer.getSample(0, i) / magnitude);
		}
	}

private:
	Wavetable waveTable;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthSound)
};