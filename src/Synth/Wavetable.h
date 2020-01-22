/*
  ==============================================================================

	Wavetable.h
	Created: 14 Aug 2019 11:43:07am
	Author:  Michael

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#define WAVE_SIZE 512
#define WAVE_SIZE_F WAVE_SIZE.0f
#define MAX_WAVE_TBL_SIZE 256
class Wavetable
{
public:
	Wavetable(AudioParameterFloat* mWaveParameter, bool initializeWaves = true) {
		if (initializeWaves) {
			createWavetable();
			//createWavetableBasicWaves();
		}
		this->mParamWave = mWaveParameter;
	}
	~Wavetable() {
		mParamWave = nullptr;
		waveTable.clear();
	}

	void setWavetableUsingBuffer(AudioSampleBuffer* buffer) {
		//must already be normalized
		waveTable.clear();
		//DBG(buffer->getNumSamples() / WAVE_SIZE);
		mWaveTableSize = buffer->getNumSamples() / WAVE_SIZE;

		//waveTable must have at least 1 frame & less or equal than max sizse
		jassert(mWaveTableSize > 0 && mWaveTableSize <= MAX_WAVE_TBL_SIZE);
		jassert(buffer->getNumChannels() == 1);
		//normalizeBuffer(buffer);

		for (int waveIndex = 0; waveIndex < mWaveTableSize; ++waveIndex) {
			Array<float> *wave = new Array<float>;
			//float min = buffer->getSample(0, WAVE_SIZE * waveIndex);
			//float max = buffer->getSample(0, WAVE_SIZE * waveIndex);
			//float prevSample = buffer->getSample(0, 0);
			
			for (int i = 0; i < WAVE_SIZE; ++i) {
				float sample = buffer->getSample(0, i + WAVE_SIZE * waveIndex);
				wave->set(i, sample);
				//wave->add((*a)[i + WAVE_SIZE * waveIndex]);
				//if (sample < min) {
				//	min = sample;
				//}
				//if(sample > max){
				//	max = sample;
				//}
			}

			//for (int i = 0; i < WAVE_SIZE; ++i) {
			//	if(min!=max){
			//		float newSamp =
			//			((*wave)[i] - min)/
			//			(max - min) * 2 - 1;
			//		wave->set(i, newSamp);
			//	}
			//	else {
			//		float newSamp = 0;
			//		wave->set(i, newSamp);
			//	}
			//}
			waveTable.add(wave);
		}
	}
	//normalize buffer, only to be used in the above function,
	//need to rewrite this code if used for smthing else!


	inline int getCurrentWaveIndex() const {
		return roundToInt(mParamWave->get()*(mWaveTableSize - 1));
	}

	Array<float>* getCurrentWave() const {
		return waveTable[getCurrentWaveIndex()];
	}

	//Array<float>* getWave(int index) {
	//	jassert(index < WAVE_TBL_SIZE);
	//	return waveTable[index];
	//}



private:
	int mWaveTableSize;
	AudioParameterFloat* mParamWave;

	Random random;
	void createWavetable() {
		if (!waveTable.isEmpty()) {
			waveTable.clear();
		}
		createWavetableBasicWaves();
		//createWavetableSinSquare();
		//createWavetableOppoSquare();
		//createWavetableNoiseNothing();
		//createWavetableTriangleNothing();
		//applyMorphTo2Waves();
		applyCrossFade();

	}
	//todo make this available for more than 2 waves

	void applyFFTBandlimiting() {

	}

	//this functions goes through 2 loops. Although slower but avoids overflow of waves
	void applyCrossFade() {
		int originalWaveTableSize = mWaveTableSize;
		mWaveTableSize = MAX_WAVE_TBL_SIZE;
		Array<float>* waveFirst = waveTable.getFirst();
		//Array<float> wave1 = *wave1Ptr;
		Array<float>* waveLast = waveTable.getLast();
		//Array<float> wave2 = *wave2Ptr;

		//interpolation variables
		Array<float> A;//index is prevWave #
		OwnedArray<Array<float>> M;//1st index is prevIndex, 2nd is sampleIndex
		OwnedArray<Array<float>> B;

		for (int waveIndex = 0; waveIndex < originalWaveTableSize - 1; ++waveIndex) {
			//waveindex still refers to oldwave
			float stretchedIndex = waveIndex * (mWaveTableSize - 1.0f) / (originalWaveTableSize - 1.0f);
			//float max = prevWaveIndex * (mWaveTableSize - 1.0f) / (prevWaveTableSize - 1.0f);
			A.add(stretchedIndex);

			Array<float> *currentM = new Array<float>;
			Array<float> *currentB = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				float rise = (*waveTable[waveIndex + 1])[i] - (*waveTable[waveIndex])[i];
				float run = (mWaveTableSize - 1.0f) / (originalWaveTableSize - 1.0f);
				float currentMSample = rise / run;
				currentM->add(currentMSample);
				float currentBSample = (*waveTable[waveIndex])[i];
				currentB->add(currentBSample);
			}
			M.add(currentM);
			B.add(currentB);
		}
		//remove waves in the middle
		waveTable.removeRange(1, originalWaveTableSize - 2);

		//creates the new waves: loops through 2nd to 2nd last
		for (int x = 1; x < mWaveTableSize - 1; ++x) {
			Array<float>* newWave = new Array<float>;
			int originalIndex = x * (originalWaveTableSize - 1.0f) / (mWaveTableSize - 1.0f);

			float a = A[originalIndex];
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				//interpolation
				float m = (*M[originalIndex])[i];
				float b = (*B[originalIndex])[i];
				float sample = m * (x - a) + b;
				//set sample
				newWave->set(i, sample);
			}
			waveTable.insert(waveTable.size() - 1, newWave);
		}


	}
	void applyMorphTo2Waves() {
		mWaveTableSize = MAX_WAVE_TBL_SIZE;
		Array<float>* wave1 = waveTable[0];
		//Array<float> wave1 = *wave1Ptr;
		Array<float>* wave2 = waveTable[1];
		//Array<float> wave2 = *wave2Ptr;

		for (int waveIndex = 1; waveIndex < mWaveTableSize - 1; ++waveIndex) {
			Array<float>* newWave = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				//interpolation
				float m = ((*wave2)[i] - (*wave1)[i]) / ((float)mWaveTableSize - 1.0f);
				float x = waveIndex;
				//DBG(d);
				float b = (*wave1)[i];
				float sample = m * x + b;
				newWave->set(i, sample);
			}
			//waveTable.add(newWave);
			waveTable.insert(waveTable.size() - 1, newWave);
		}
		//waveTable.move(1, -1);
	}
	//===== INIT WAVETABLES ===== NOTHING FANCY HERE
	enum BasicWaveType
	{
		SawWave, SquareWave, SineWave, TriangleWave, TriSawWave, HalfPulseSquareWave, QuarterPulseSquareWave, Noise
	};
	float getCurrentSampleAccordingToBasicWaveType(BasicWaveType type, int i) {
		float sample;
		switch (type) {
		case SineWave:
		{
			sample = sinf(i * 2.0f * float_Pi / WAVE_SIZE);
		}
		break;
		case SquareWave:
		{
			if (i < ((WAVE_SIZE - 1.0f) * 0.5f)) {
				sample = -1.0f;
			}
			else {
				sample = 1.0f;
			}
		}
		break;
		case HalfPulseSquareWave:
		{
			if (i < ((WAVE_SIZE - 1.0f) * 0.25f)) {
				sample = -1.0f;
			}
			else {
				sample = 1.0f;
			}
		}
		break;
		case QuarterPulseSquareWave:
		{
			if (i < ((WAVE_SIZE - 1.0f) * 0.125f)) {
				sample = -1.0f;
			}
			else {
				sample = 1.0f;
			}
		}
		break;
		case SawWave:
		{
			float m = 2.0f / WAVE_SIZE;
			if (i < ((WAVE_SIZE - 1.0f) * 0.5f)) {
				sample = m * i;
			}
			else {
				sample = m * i - 2.0f;
			}
		}
		break;
		case TriangleWave:
		{
			float m = 4.0f / WAVE_SIZE;
			if (i < ((WAVE_SIZE - 1.0f) * 0.25f)) {
				sample = m * i;
			}
			else if (i >= ((WAVE_SIZE - 1.0f) * 0.25f) &&
				i <= ((WAVE_SIZE - 1.0f) * 0.75f)) {
				sample = -m * i + 2.0f;
			}
			else {
				sample = m * i - 4.0f;
			}
		}
		break;
		case TriSawWave:
		{
			float m = 2.0f / WAVE_SIZE;
			if (i < ((WAVE_SIZE - 1.0f) * 0.5f)) {
				sample = -m * i + 1.0f;
			}
			else {
				sample = m * i - 2.0f;
			}
		}
		break;
		case Noise:
		{
			sample = random.nextFloat()*2.0 - 1.0f;
		}
		break;
		default:
		{
			sample = 0;
		}
		break;
		};
		return sample;
	}
	void createWavetableBasicWaves()
	{
		mWaveTableSize = Noise + 1;//todo: trick
		for (int tblPos = 0; tblPos < mWaveTableSize; ++tblPos) {
			Array<float>* wave = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				float sample = getCurrentSampleAccordingToBasicWaveType(static_cast<BasicWaveType>(tblPos), i);
				wave->set(i, sample);
			}
			waveTable.add(wave);
		}
	}
	void createWavetableSinSquare() {
		mWaveTableSize = 2;
		for (int tblPos = 0; tblPos < mWaveTableSize; ++tblPos) {
			Array<float>* wave = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				float sample;
				if (tblPos == 0) {
					sample = getCurrentSampleAccordingToBasicWaveType(SineWave, i);
				}
				else {
					sample = getCurrentSampleAccordingToBasicWaveType(SquareWave, i);
				}
				wave->set(i, sample);
			}
			waveTable.add(wave);
		}
	}
	void createWavetableOppoSquare() {
		mWaveTableSize = 2;
		for (int tblPos = 0; tblPos < mWaveTableSize; ++tblPos) {
			Array<float>* wave = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				float sample;
				if (tblPos == 0) {
					sample = getCurrentSampleAccordingToBasicWaveType(SquareWave, i);
				}
				else {
					sample = -getCurrentSampleAccordingToBasicWaveType(SquareWave, i);
				}
				wave->set(i, sample);
			}
			waveTable.add(wave);
		}
	}
	void createWavetableNoiseNothing() {
		mWaveTableSize = 2;
		for (int tblPos = 0; tblPos < mWaveTableSize; ++tblPos) {
			Array<float>* wave = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				float sample;
				if (tblPos == 0) {
					sample = getCurrentSampleAccordingToBasicWaveType(Noise, i);
				}
				else {
					sample = 0;
				}
				wave->set(i, sample);
			}
			waveTable.add(wave);
		}
	}
	void createWavetableTriangleNothing() {
		mWaveTableSize = 2;
		for (int tblPos = 0; tblPos < mWaveTableSize; ++tblPos) {
			Array<float>* wave = new Array<float>;
			for (int i = 0; i < WAVE_SIZE; ++i)
			{
				float sample;
				if (tblPos == 0) {
					sample = getCurrentSampleAccordingToBasicWaveType(TriangleWave, i);
				}
				else {
					sample = 0;
				}
				wave->set(i, sample);
			}
			waveTable.add(wave);
		}
	}
	OwnedArray<Array<float>> waveTable;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Wavetable)
};