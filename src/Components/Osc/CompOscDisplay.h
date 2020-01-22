/*
  ==============================================================================

    CompOscDisplay.h
    Created: 13 Aug 2019 1:12:07pm
    Author:  Michael

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "ParamIDs.h"
//==============================================================================
/*
*/
class CompOscDisplay    : public Component,
	//public Slider::Listener,
	public AudioProcessorValueTreeState::Listener,
	public AsyncUpdater
{
public:
    CompOscDisplay(SynthSound* sound)
    {
		//vts.addParameterListener(ID_osc1_wave, this);
		//added listener in parent class
		this->sound = sound;
		wave = this->sound->getCurrentWave();

		//getParentComponent().
		//DBG(wave->);
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~CompOscDisplay()
    {
		sound = nullptr;
		wave = nullptr;
    }

    void paint (Graphics& g) override
	{
		g.fillAll(Colours::black);
		Rectangle<float> waveBounds(getLocalBounds().toFloat()/*.reduced(10)*/);
		//draw
		Path path;
		for (int i = 0; i < WAVE_SIZE; i++/*i+= WAVE_SIZE/256*/) {
			float sampleVal = (*wave)[i];
			float x = i/ WAVE_SIZE_F * waveBounds.getWidth();// / WAVE_SIZE * getWidth();
			float y = (waveBounds.getHeight() / 2)*(1 - sampleVal);//-1*(sampleVal*getHeight()/2) + getHeight()/2;
			Point<float> point(x,y);
			if (i == 0) {
				path.startNewSubPath(point);
			}
			else {
				path.lineTo(point);
			}
		}
		//path.closeSubPath();
		g.setColour(Colours::orangered);
		g.strokePath(path, PathStrokeType(1));
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }


	void parameterChanged(const String& parameterID, float newValue) override {
		triggerAsyncUpdate();
	}

	void handleAsyncUpdate() override {
		wave = sound->getCurrentWave();
		repaint();
	}

private:
	Array<float>* wave;
	SynthSound* sound;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompOscDisplay)
};
