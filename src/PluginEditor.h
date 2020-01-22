#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CompOsc1.h"
//==============================================================================
/**
*/
class ArcAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ArcAudioProcessorEditor (ArcAudioProcessor&,
		AudioProcessorValueTreeState&);
    ~ArcAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArcAudioProcessor& processor;
    CompOsc1 mCompOsc1;
	//CompOscDisplay mCompOscDisplay;
	
	//LOOK AND FEEL
	class OfficialLookAndFeel : public LookAndFeel_V4 {
	public:
		// ===== RotarySlider ===== //

		const Colour mainColor = Colours::blueviolet.brighter();//azure;//blueviolet
		//knob Colors
		const Colour knobColor = Colours::darkgrey.darker().darker();
		const Colour voidColor = Colours::black;

		void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos,
			float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override {

			//init variables
			auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);
			//auto centreX = x + width * 0.5f;
			//auto centreY = y + height * 0.5f;
			auto centre = bounds.getCentre();

			bounds.setSize(jmin(bounds.getWidth(), bounds.getHeight()), jmin(bounds.getWidth(), bounds.getHeight()));
			bounds.setCentre(centre);

			auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
			auto knobBounds = bounds.reduced(5);
			auto knobRadius = jmin(knobBounds.getWidth(), knobBounds.getHeight()) / 2.0f;

			auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

			//Black Background
			g.setColour(voidColor);
			g.fillEllipse(bounds);
			//Knob

			g.setColour(knobColor);
			g.fillEllipse(knobBounds);
			//GRADIENT: Knob
			//setVerticalGradient(g, bounds, knobColor, true, 0.05f);
			//g.fillEllipse(bounds.reduced(9.7f));
			//Arc

			//valueRing

			auto lineW = radius - knobRadius;
			auto arcRadius = radius - lineW * 0.5f;
			if (slider.isEnabled())
			{
				Path valueArc;
				valueArc.addCentredArc(bounds.getCentreX(),
					bounds.getCentreY(),
					arcRadius,
					arcRadius,
					0.0f,
					rotaryStartAngle,
					angle,
					true);
				//STATIONARY g.setColour(mainColor);
				//GRADIENT: RotarySlider
				setMainHorizontalGradient(g, bounds, false);
				//g.setGradientFill(ColourGradient(Colours::white,))
				//setHorizontalGradient(g, bounds, mainColor, false);

				//setMainRadialGradient(g, bounds);
				//g.fillRect(bounds);
				//setHorizontalGradient(g, bounds.withWidth(bounds.getWidth()*1.5), mainColor.darker(0.5f), Colours::azure);
				g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved));
			}

			//pointer
			Path p;
			auto pointerLength = knobRadius * 0.4f;
			auto pointerThickness = 1.0f;
			p.addRoundedRectangle(-pointerThickness * 0.5f, -knobRadius * 0.85, pointerThickness, pointerLength, 3);
			p.applyTransform(AffineTransform::rotation(angle).translated(centre));

			// pointer
			g.setColour(Colours::white);
			g.fillPath(p);
		}

		// ===== Popup(bubble) ===== //
		void drawBubble(Graphics& g, BubbleComponent& comp,
			const Point<float>& tip, const Rectangle<float>& body) override
		{
			//bubble path
			Path p;
			p.addBubble(body.reduced(0.5f), body.getUnion(Rectangle<float>(tip.x, tip.y, 1.0f, 1.0f)),
				tip, 5.0f, jmin(15.0f, body.getWidth() * 0.2f, body.getHeight() * 0.2f));
			g.setColour(Colours::darkgrey.darker().darker().darker().withAlpha(0.8f));
			g.fillPath(p);
		}

		//horizontalSlider Colors
		const Colour lightBackgroundColor = Colours::grey;

		// ===== LinearSlider(including bar) ===== //
		void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
			float sliderPos,
			float minSliderPos,
			float maxSliderPos,
			const Slider::SliderStyle style, Slider& slider) override
		{

			if (slider.isBar()) {
				mDrawBarLinearSlider(g, x, y, width, height,
					sliderPos, slider);
			}
			else {
				//ASSUMING ONLY ONE VALUE
				auto isTwoVal = (style == Slider::SliderStyle::TwoValueVertical || style == Slider::SliderStyle::TwoValueHorizontal);
				auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

				if (isTwoVal || isThreeVal) {
					LookAndFeel_V4::drawLinearSlider(g, x, y, width, height,
						sliderPos,
						minSliderPos,
						maxSliderPos,
						style, slider);
					return;
				}
				mDrawThinLinearSlider(g, x, y, width, height,
					sliderPos, slider);
			}
		}

		// ===== CUSTOM FUNCTIONS=====

		void mDrawBarLinearSlider(Graphics& g, int& x, int& y, int& width, int& height,
			float& sliderPos, Slider& slider) {

			//background track
			Rectangle<float> bounds = slider.getBounds().toFloat();

			Rectangle<float> bgTrack(x, y, width, height);
			g.setColour(lightBackgroundColor);
			g.fillRect(squishedVertical(bgTrack));

			//valueTrack
			Rectangle<float> valueTrack = slider.isHorizontal() ? Rectangle<float>(static_cast<float> (x), y, sliderPos - x, height)
				: Rectangle<float>(x, sliderPos, width, y + (height - sliderPos));
			//setHorizontalGradient(g, valueTrack, mainColor);
			//GRADIENT: BarLinearSlider
			setMainHorizontalGradient(g, Rectangle<float>(x, y, width, height));
			g.fillRect(squishedVertical(valueTrack));
			//g.fillRect(shrunk(valueTrack));

			if (slider.isHorizontal()) {
				float handleWidth = height * 0.30f;//jmin(5.0f, height * 0.30f);
				//Rectangle<float> handleBg(x - handleWidth, y, x + handleWidth, height);

				float handlePos;
				if (sliderPos - handleWidth <= 0) {
					handlePos = 0;
				}
				else {
					handlePos = sliderPos - x - handleWidth * 0.8;
				}
				Rectangle<float> handleBg(handlePos, y, handleWidth, height);
				g.setColour(knobColor);
				g.fillRect(handleBg);
			}
			else {
				//TODO: PROVIDE DEFINITION FOR VERTICLE
			}

		}

		Rectangle<float> squishedVertical(Rectangle<float> &rect, float factor = 0.9f) {
			float trim = rect.getHeight()*(1 - factor) / 2;
			return rect.withTrimmedBottom(trim).withTrimmedTop(trim);
		}

		Rectangle<float> squishedHorizontal(Rectangle<float> &rect, float factor = 0.9f) {
			float trim = rect.getWidth()*(1 - factor) / 2;
			return rect.withTrimmedLeft(trim).withTrimmedRight(trim);
		}

		Rectangle<float> shrunk(Rectangle<float> &rect, float factor = 0.9f) {
			return rect.withSizeKeepingCentre(rect.getWidth()*factor, rect.getHeight()*factor);
		}

		//setting fills
		void setVerticalGradient(Graphics& g, const Rectangle<float>& area,
			const Colour& baseColour, bool topBrightBottomDark = true, const float colorShiftAmount = 0.5f)//, bool brightTopDarkBottom)
		{
			if (topBrightBottomDark) {
				g.setGradientFill(ColourGradient(baseColour.brighter(colorShiftAmount), 0,
					(float)area.getY(), baseColour.darker(colorShiftAmount), 0,
					(float)area.getBottom(), false));
			}
			else {
				g.setGradientFill(ColourGradient(baseColour.darker(colorShiftAmount), 0,
					(float)area.getY(), baseColour.brighter(colorShiftAmount), 0,
					(float)area.getBottom(), false));
			}
		}

		void setMainHorizontalGradient(Graphics& g, const Rectangle<float>& area,
			bool brightLeftDarkRight = true, const float darker = 0.5f)
		{
			if (brightLeftDarkRight) {
				g.setGradientFill(ColourGradient(mainColor, (float)area.getX(), 0,
					mainColor.darker(darker), (float)area.getRight(), 0, false));
			}
			else {
				g.setGradientFill(ColourGradient(mainColor.darker(darker), (float)area.getX(), 0,
					mainColor, (float)area.getRight(), 0, false));
			}
		}

		//TODO: BRIGHTLEFTDARKRIGHT
		void setMainRadialGradient(Graphics& g, const Rectangle<float>& area,
			/*bool brightLeftDarkRight = true, */float darker = 0.5f)
		{
			g.setGradientFill(ColourGradient(mainColor, area.getCentre(),
				mainColor.darker(darker), area.getBottomRight(), true));
		}

		void setHorizontalGradient(Graphics& g, const Rectangle<float>& area,
			const Colour& startColor, const Colour& endColor)
		{
			g.setGradientFill(ColourGradient(startColor, (float)area.getX(), 0,
				endColor, (float)area.getRight(), 0, false));
		}

		void setHorizontalGradient(Graphics& g, const Rectangle<float>& area,
			const Colour& baseColour, bool brightLeftDarkRight, float amount = 0.5f)
		{
			if (brightLeftDarkRight) {
				g.setGradientFill(ColourGradient(baseColour.brighter(amount), (float)area.getX(), 0,
					baseColour.darker(amount), (float)area.getRight(), 0, false));
			}
			else {
				g.setGradientFill(ColourGradient(baseColour.darker(amount), (float)area.getX(), 0,
					baseColour.brighter(amount), (float)area.getRight(), 0, false));
			}
		}

		void setCoolContrastGradient(Graphics& g, const Rectangle<float>& area,
			const Colour& baseColour, bool brightLeftDarkRight)
		{
			g.setGradientFill(ColourGradient(baseColour.brighter(-0.5f), (float)area.getX(), 0,
				baseColour.darker(0.5f), (float)area.getRight(), 0, false));
		}

		//end of setting fills
		void mDrawThinLinearSlider(Graphics& g, int& x, int& y, int& width, int& height,
			float& sliderPos, Slider& slider) {
			auto trackWidth = jmin(6.0f, slider.isHorizontal() ? height * 0.30f : width * 0.30f);//jmin(6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);

			Point<float> startPoint(slider.isHorizontal() ? x : x + width * 0.5f,
				slider.isHorizontal() ? y + height * 0.5f : height + y);

			Point<float> endPoint(slider.isHorizontal() ? width + x : startPoint.x,
				slider.isHorizontal() ? startPoint.y : y);

			//background track
			Path backgroundTrack;
			backgroundTrack.startNewSubPath(startPoint);
			backgroundTrack.lineTo(endPoint);
			g.setColour(lightBackgroundColor);
			g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });
			auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(1);

			//value track
			Path valueTrack;
			Point<float> minPoint, maxPoint;
			auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
			auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;
			minPoint = startPoint;
			maxPoint = { kx, ky };

			valueTrack.startNewSubPath(minPoint);
			valueTrack.lineTo(maxPoint);
			//g.setColour(mainColor);
			//GRADIENT: ThinLinearSlider
			setMainHorizontalGradient(g, bounds);
			g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

			//thumb
			auto backThumbWidth = getSliderThumbRadius(slider);
			g.setColour(lightBackgroundColor.darker().darker());
			g.fillEllipse(Rectangle<float>(static_cast<float> (backThumbWidth), static_cast<float> (backThumbWidth)).withCentre(maxPoint));

			auto thumbWidth = getSliderThumbRadius(slider)*0.5;
			g.setColour(Colours::white);
			g.fillEllipse(Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(maxPoint));
		}
	};
	//END OF LOOKANDFEEL

	OfficialLookAndFeel mLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArcAudioProcessorEditor)
};
