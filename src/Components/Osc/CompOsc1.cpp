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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "CompOsc1.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CompOsc1::CompOsc1 (AudioProcessorValueTreeState& vts, SynthSound* sound)
{
    //[Constructor_pre] You can add your own custom stuff here..
	//^OscDisplay initialized already

    //[/Constructor_pre]

    toggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton.get());
    toggleButton->setButtonText (TRANS("Osc 1"));
    toggleButton->addListener (this);

    toggleButton->setBounds (7, 6, 150, 24);

    mCompOscDisplay.reset (new CompOscDisplay (sound));
    addAndMakeVisible (mCompOscDisplay.get());
    mCompOscDisplay->setName ("new component");

    mCompOscDisplay->setBounds (10, 39, 280, 201);

    slider.reset (new Slider ("Osc1 Wide"));
    addAndMakeVisible (slider.get());
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    slider->setBounds (152, 336, 64, 64);

    slider4.reset (new Slider ("Osc1 Volume"));
    addAndMakeVisible (slider4.get());
    slider4->setRange (0, 10, 0);
    slider4->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider4->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    slider4->setBounds (231, 334, 64, 64);

    mSliderVoices.reset (new Slider ("Osc1 Voices"));
    addAndMakeVisible (mSliderVoices.get());
    mSliderVoices->setRange (1, 10, 1);
    mSliderVoices->setSliderStyle (Slider::LinearBarVertical);
    mSliderVoices->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    mSliderVoices->addListener (this);

    mSliderVoices->setBounds (239, 6, 54, 24);

    label.reset (new Label ("new label",
                            TRANS("Voices")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (191, 6, 48, 24);

    mSliderWaveNum.reset (new Slider ("Osc1 WaveNum"));
    addAndMakeVisible (mSliderWaveNum.get());
    mSliderWaveNum->setRange (0, 10, 0);
    mSliderWaveNum->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mSliderWaveNum->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    mSliderWaveNum->setBounds (7, 242, 64, 64);

    mSliderDetune.reset (new Slider ("Osc1 Detune"));
    addAndMakeVisible (mSliderDetune.get());
    mSliderDetune->setRange (0, 10, 0);
    mSliderDetune->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mSliderDetune->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    mSliderDetune->setBounds (151, 242, 64, 64);

    mSliderPan.reset (new Slider ("Osc1 Pan"));
    addAndMakeVisible (mSliderPan.get());
    mSliderPan->setRange (0, 10, 0);
    mSliderPan->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mSliderPan->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    mSliderPan->setBounds (231, 242, 64, 64);

    label3.reset (new Label ("new label",
                             TRANS("Volume\n")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (239, 398, 64, 24);

    label4.reset (new Label ("new label",
                             TRANS("Pan")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (247, 302, 32, 24);

    label5.reset (new Label ("new label",
                             TRANS("Wave")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (7, 302, 64, 24);

    label6.reset (new Label ("new label",
                             TRANS("Detune\n")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (159, 302, 56, 24);

    label7.reset (new Label ("new label",
                             TRANS("Wide")));
    addAndMakeVisible (label7.get());
    label7->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (168, 400, 40, 24);

    mSliderPitch.reset (new Slider ("Osc1 Pitch"));
    addAndMakeVisible (mSliderPitch.get());
    mSliderPitch->setRange (0, 10, 0);
    mSliderPitch->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    mSliderPitch->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    mSliderPitch->setBounds (81, 241, 64, 64);

    slider2.reset (new Slider ("Osc1 Phase"));
    addAndMakeVisible (slider2.get());
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider2->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    slider2->setBounds (10, 332, 64, 64);

    label8.reset (new Label ("new label",
                             TRANS("Phase")));
    addAndMakeVisible (label8.get());
    label8->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centred);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label8->setBounds (10, 396, 64, 24);

    slider3.reset (new Slider ("Osc1 Fine"));
    addAndMakeVisible (slider3.get());
    slider3->setRange (0, 10, 0);
    slider3->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider3->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

    slider3->setBounds (78, 333, 64, 64);

    label10.reset (new Label ("new label",
                              TRANS("Fine")));
    addAndMakeVisible (label10.get());
    label10->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label10->setBounds (78, 397, 64, 24);

    textButton.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("Pitch"));
    textButton->addListener (this);
    textButton->setColour (TextButton::buttonColourId, Colour (0xff5e5e5e));

    textButton->setBounds (83, 304, 56, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 430);


    //[Constructor] You can add your own custom stuff here..
	for (Component* compPtr : getChildren()) {
		if (instanceof<Slider>(compPtr)) {
			Slider* sliderPtr = dynamic_cast<Slider*>(compPtr);
			//===CreateAttachments


			if (sliderPtr->getName() == ID_osc1_wave ||

				sliderPtr->getName() == ID_osc1_voices_int||
				sliderPtr->getName() == ID_osc1_phase ||

				sliderPtr->getName() == ID_osc1_pitch ||
				sliderPtr->getName() == ID_osc1_fine ||

				sliderPtr->getName() == ID_osc1_detune||
				sliderPtr->getName() == ID_osc1_wide ||

				sliderPtr->getName() == ID_osc1_pan ||
				sliderPtr->getName() == ID_osc1_volume
				) {
				//DBG("SUCCESS!");
				//slider.onValueChange = [this, parent, &slider]{
				//	parent->setScaleFactor(slider.getValue()/100.0f);
				//};
				mSliderAttachments.add(new AudioProcessorValueTreeState::SliderAttachment(vts, sliderPtr->getName(), *sliderPtr));
				sliderPtr->textFromValueFunction = nullptr;
				sliderPtr->setNumDecimalPlacesToDisplay(2);
				sliderPtr->setPopupDisplayEnabled(true, true, this);
			}
			else {
				//mSliderAttachments.add(new AudioProcessorValueTreeState::SliderAttachment(vts, slider.getName(), slider));


				////DBG: FOR DEBUG PURPOSES ONLY, TODOFUTURE: REMOVE
				////DBG(vts.getParameter(slider.getName())->name);
				////ERROR: <" IF THIS HAPPENED YOU DID NOT MAKE ALL PARAMS! "
				////end of DBG
				////===Other Inits
				////mSliderAttachments.

				//slider.textFromValueFunction = nullptr;
				//slider.setNumDecimalPlacesToDisplay(2);
				//slider.setPopupDisplayEnabled(true, true, this);
			}
		}
	}
	//mSliderWaveNum->addListener(mCompOscDisplay.get());
	vts.addParameterListener(ID_osc1_wave, mCompOscDisplay.get());
	vtsPtr = &vts;
    //[/Constructor]
}

CompOsc1::~CompOsc1()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	vtsPtr->removeParameterListener(ID_osc1_wave, mCompOscDisplay.get());
	mSliderAttachments.clear();
	vtsPtr = nullptr;
    //[/Destructor_pre]

    toggleButton = nullptr;
    mCompOscDisplay = nullptr;
    slider = nullptr;
    slider4 = nullptr;
    mSliderVoices = nullptr;
    label = nullptr;
    mSliderWaveNum = nullptr;
    mSliderDetune = nullptr;
    mSliderPan = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    mSliderPitch = nullptr;
    slider2 = nullptr;
    label8 = nullptr;
    slider3 = nullptr;
    label10 = nullptr;
    textButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CompOsc1::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CompOsc1::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void CompOsc1::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton.get())
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void CompOsc1::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == mSliderVoices.get())
    {
        //[UserSliderCode_mSliderVoices] -- add your slider handling code here..
        //[/UserSliderCode_mSliderVoices]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CompOsc1" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorValueTreeState&amp; vts, SynthSound* sound"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="300" initialHeight="430">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TOGGLEBUTTON name="new toggle button" id="aac5b28e82389a63" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="7 6 150 24" buttonText="Osc 1"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="new component" id="5d121147827e2e0e" memberName="mCompOscDisplay"
                    virtualName="" explicitFocusOrder="0" pos="10 39 280 201" class="CompOscDisplay"
                    params="sound"/>
  <SLIDER name="Osc1 Wide" id="3d0046bebbf53470" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="152 336 64 64" min="0.0" max="10.0"
          int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Osc1 Volume" id="8787aa2b5b9a3f8b" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="231 334 64 64" min="0.0"
          max="10.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Osc1 Voices" id="a2f4b986dd5ee6bf" memberName="mSliderVoices"
          virtualName="" explicitFocusOrder="0" pos="239 6 54 24" min="1.0"
          max="10.0" int="1.0" style="LinearBarVertical" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="d3ab4407bea2f47a" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="191 6 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Voices" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Osc1 WaveNum" id="cccc643a73462411" memberName="mSliderWaveNum"
          virtualName="" explicitFocusOrder="0" pos="7 242 64 64" min="0.0"
          max="10.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Osc1 Detune" id="4deed0535c27a4c8" memberName="mSliderDetune"
          virtualName="" explicitFocusOrder="0" pos="151 242 64 64" min="0.0"
          max="10.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Osc1 Pan" id="ade496301df280e4" memberName="mSliderPan"
          virtualName="" explicitFocusOrder="0" pos="231 242 64 64" min="0.0"
          max="10.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="482bab30ee94594" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="239 398 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Volume&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="387bd9213ee5934f" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="247 302 32 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Pan" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="1ec4eb718f53a588" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="7 302 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Wave" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="ef4fbe42561e295c" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="159 302 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Detune&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="65254ebecdcf523c" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="168 400 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Wide" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="Osc1 Pitch" id="75206ececfdd8146" memberName="mSliderPitch"
          virtualName="" explicitFocusOrder="0" pos="81 241 64 64" min="0.0"
          max="10.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Osc1 Phase" id="367ab83eced17532" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="10 332 64 64" min="0.0"
          max="10.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="6937f38429f776ac" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="10 396 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Phase" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="Osc1 Fine" id="a5e0b14571b6f93b" memberName="slider3" virtualName=""
          explicitFocusOrder="0" pos="78 333 64 64" min="0.0" max="10.0"
          int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="0" textBoxHeight="0" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="199aa573c7493b3a" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="78 397 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Fine" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="1ee18664380f7d2a" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="83 304 56 24" bgColOff="ff5e5e5e"
              buttonText="Pitch" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

