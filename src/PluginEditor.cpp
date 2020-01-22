#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ArcAudioProcessorEditor::ArcAudioProcessorEditor (ArcAudioProcessor& p,
	AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p),
	mCompOsc1(vts, p.mSynthSound)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    LookAndFeel_V4::setDefaultLookAndFeel(&mLookAndFeel);

    setSize (800, 430);
    mCompOsc1.setBounds(400, 0, 400, 430);
	addAndMakeVisible(mCompOsc1);
    
}

ArcAudioProcessorEditor::~ArcAudioProcessorEditor()
{
}

//==============================================================================
void ArcAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void ArcAudioProcessorEditor::resized()
{

}
