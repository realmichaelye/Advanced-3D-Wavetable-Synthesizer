/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "CompOscDisplay.h"
#include "ParamIDs.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CompOsc1  : public Component,
                  public Button::Listener,
                  public Slider::Listener
{
public:
    //==============================================================================
    CompOsc1 (AudioProcessorValueTreeState& vts, SynthSound* sound);
    ~CompOsc1();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    template<typename ObjectClass, typename T>
	static inline bool instanceof(const T *ptr) {
		return dynamic_cast<const ObjectClass*>(ptr) != nullptr;
	}

	//TODO: MAKE OWNEDARRAY FOR ATTACHMENTS & automatically load using initSliders function(w param id)
	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> mSliderAttachments;
	AudioProcessorValueTreeState* vtsPtr;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ToggleButton> toggleButton;
    std::unique_ptr<CompOscDisplay> mCompOscDisplay;
    std::unique_ptr<Slider> slider;
    std::unique_ptr<Slider> slider4;
    std::unique_ptr<Slider> mSliderVoices;
    std::unique_ptr<Label> label;
    std::unique_ptr<Slider> mSliderWaveNum;
    std::unique_ptr<Slider> mSliderDetune;
    std::unique_ptr<Slider> mSliderPan;
    std::unique_ptr<Label> label3;
    std::unique_ptr<Label> label4;
    std::unique_ptr<Label> label5;
    std::unique_ptr<Label> label6;
    std::unique_ptr<Label> label7;
    std::unique_ptr<Slider> mSliderPitch;
    std::unique_ptr<Slider> slider2;
    std::unique_ptr<Label> label8;
    std::unique_ptr<Slider> slider3;
    std::unique_ptr<Label> label10;
    std::unique_ptr<TextButton> textButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompOsc1)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

