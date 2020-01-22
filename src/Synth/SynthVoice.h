/*
  ==============================================================================

    SynthVoice.h
    Created: 9 Aug 2019 7:44:20pm
    Author:  Michael

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#define VOICE_MAX_NUMBER 16
#define VOICE_PITCH_MAX 2400.0f//2 octaves (in cents)
#define VOICE_DETUNE_MAX 120.0f//cents
#define VOICE_VOLUME_MULTIPLIER 0.2f

class SynthVoice : public SynthesiserVoice
{
public:
	bool canPlaySound(SynthesiserSound* sound) {
		//DBG("CANPLAYSOUND");
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	//===============================================================

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) {
		//DBG("startnote");
		//env1.trigger = 1;
		//TODO: pitch note changer
		this->sound = dynamic_cast<SynthSound*>(sound);
		wave = this->sound->getCurrentWave();
		mLevel = velocity;
		mFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

		int numVoices/*paramVoices*/ = this->sound->mParamVoices->get();
		//===phase
		float paramPhase = this->sound->mParamPhase->get();
		if (paramPhase < 99.99f) {
			mPhase = paramPhase * getSampleRate() / mFrequency;
		}
		//use memphase if bigger than 99.99f
		//===WIDE
		float paramWide = this->sound->mParamWide->get();
		mPhaseIncrements.clear();
		for (int v = 0; v < VOICE_MAX_NUMBER; ++v) {
			float phaseIncrement = paramWide*random.nextFloat()*getSampleRate()/mFrequency;
			mPhaseIncrements.add(phaseIncrement);
		}
		
	}

	void stopNote(float velocity, bool allowTailOff) {
		//allowTailOff = true;
		
		if (velocity == 0){
			clearCurrentNote();
		}
		mLevel = 0;
	}

	//process===============================================================
	//const float ConstantLn2Div1200 = logf(2) / 1200.0f;

	inline float withAddedCents (float frequency, float cents) const noexcept {
		return powf(2.0f, cents / 1200.0f) * frequency;
		//return dsp::FastMathApproximations::exp(ConstantLn2Div1200 * cents) * frequency;
	}
	
	void renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples)/* noexcept*/{

		if (mLevel <= 0) {
			return;
		}
		const float sampleRate = getSampleRate();

		//==params
		const int numVoices/*paramVoices*/ = sound->mParamVoices->get();
		//float paramPhase = sound->mParamPhase->get();

		const float paramPitch = sound->mParamPitch->get();
		const float paramFine = sound->mParamFine->get();

		//float modifiedFreq = withAddedCents(mFrequency, paramFine+paramPitch*VOICE_PITCH_MAX);
		const float freqIncrement = paramFine + paramPitch * VOICE_PITCH_MAX;

		const float paramDetune = sound->mParamDetune->get();
		const float paramWide = sound->mParamWide->get();
		
		const float paramPan = sound->mParamPan->get();
		const float paramVolume = sound->mParamVolume->get();

		const float volMultiplier = VOICE_VOLUME_MULTIPLIER * paramVolume / sqrtf(numVoices);

		//===DETUNE===//
		for (int v = 0; v < numVoices; ++v) {
			if (numVoices > 1) {
				const float detunePercent = (v - (numVoices - 1.0f) / 2.0f) /
					((numVoices - 1.0f) / 2.0f);
				const float detunedCents = paramDetune * VOICE_DETUNE_MAX * detunePercent;
				mFrequencies.set(v, withAddedCents(mFrequency, detunedCents + freqIncrement));
				//modifiedFreq * powf(2, detunedCents / 1200);
			}
			else {
				mFrequencies.set(v, withAddedCents(mFrequency, freqIncrement));
			}
		}
		//==\DETUNE===//

		
		//==\params

		for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
			//v stands for voice
			float newSample=0.0f;
			for (int v = 0; v < numVoices; ++v){
				//===WIDE===//
				const float vPhase = mPhase + mPhaseIncrements[v];
				//==\WIDE===//

				//GETTING WAVETABLE
				//phase to wave index conversion
				const float index = fmodf(
					(vPhase * mFrequencies[v] * (WAVE_SIZE) / sampleRate),
					WAVE_SIZE);
				//same as above
				const int prevIndex = (int)index;

				//float index = (phase * frequency*(WAVE_SIZE) / sampleRate); //same as above
				//index = fmodf(index, WAVE_SIZE);
				//int prevIndex = (int)index;

				//linear interpolation
				const float m = (*wave)[(prevIndex + 1) & (WAVE_SIZE-1)] - (*wave)[(prevIndex)];
				const float d = index - prevIndex;
				const float b = (*wave)[prevIndex];

				//paramVol and numvoices change volume & energy
				newSample += (m * d + b);
			}

			//TODO: left right channels
			//https://forum.juce.com/t/how-do-stereo-panning-knobs-work/25773/10

			//todo: learn what this means
			const float leftSample = dsp::FastMathApproximations::cos(0.25f * float_Pi * (1.0f + paramPan)) * newSample * volMultiplier;
			const float rightSample = dsp::FastMathApproximations::cos(0.25f * float_Pi * (1.0f - paramPan)) * newSample * volMultiplier;

			outputBuffer.addSample(0, startSample, leftSample);
			outputBuffer.addSample(1, startSample, rightSample);

			++mPhase;
			++startSample;
		}
		//TODO: CPU Hog?
		wave = sound->getCurrentWave();
		
	}

	//extra===============================================================

	void pitchWheelMoved(int newPitchWheelValue) {

	}

	void controllerMoved(int controllerNumber, int newControllerValue) {

	}

	//ADSR Funcs =====
	

private:

	SynthSound* sound;
	Array<float>* wave;
	float mLevel;
	float mFrequency;
	float mPhase;

	Random random;

	Array<float, DummyCriticalSection, VOICE_MAX_NUMBER> mFrequencies;
	Array<float> mPhaseIncrements;
	
	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoice)
};