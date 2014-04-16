//
//  SettingsScreen.h
//  melodizer
//
//  Created by Damien Di Fede on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef melodizer_SettingsScreen_h
#define melodizer_SettingsScreen_h

#include "ofMain.h"
#include <vector>
#include "Settings.h"
#include "Box.h"
#include "KeyChooser.h"
#include "ScaleChooser.h"
#include "ofTrueTypeFont.h"
#include "ValueKnob.h"

// forward declair.
namespace Minim 
{
    class Waveform;
    class Summer;
}

class ValueSlider
{
public:
    ValueSlider( float x, float y, float w, float h, int hue, float* v ) 
    : mBox( x, y, w, h )
    , mHue(hue)
    , mValue(v)
    , mOutStart(0)
    , mOutRange(1)
    , mTouch(-1)
    {
    }
    
    void setRange( const float start, const float end )
    {
        mOutStart = start;
        mOutRange = end - start;
    }
    
    void draw();
    
    Box& box() { return mBox; }
    
    // returns true if handled
    bool handleTouch( const int id, const float x, const float y );
    bool handleTouchUp( const int id, const float x, const float y );
    
private:
    
    int     mHue;
    Box     mBox;
    float*  mValue;
    float   mOutStart;
    float   mOutRange;
    int     mTouch; // the touch we are currently tracking
};

class WaveformButton
{
public:
    WaveformButton( float x, float y, float w, float h, WaveformType myType, WaveformType* refType, Minim::Waveform* wave );
    
    void draw( float anim );
    
    bool handleTouch( const int id, const float x, const float y );
    bool handleTouchUp( const int id, const float x, const float y );
    
private:
    
    int                 mTouch;
    Box                 mBox;
    WaveformType        mType;
    WaveformType*       mRefType;
    Minim::Waveform*    mWave;
    ofPolyline          mLine;
    
};

// modal, non-apple UI for futzing with settings
class SettingsScreen 
{
public:
    SettingsScreen();
    ~SettingsScreen();
    
    bool needsSetup() const { return mOtherSliders.empty(); }
    
    void setup();
    void update( const float dt );
    void draw();
    
    void toneBegan(Minim::Summer* onBus, const int onTick);
    
    void show();
    void hide();
    bool active() const;
    
    void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);
    
    ofTrueTypeFont& UIFont() { return mToggleFont; }
    
private:
    
    enum
    {
        ST_SHOWING, // animate in
        ST_SHOWN,
        ST_HIDING,
        ST_HIDDEN
    } mState;
    
    float mAnimTimer;
    float mMinX, mMaxX, mMinY, mMaxY;
    
    std::vector<ValueSlider>    mMelodySliders;
    std::vector<ValueSlider>    mBassSliders;
    std::vector<ValueSlider>    mOtherSliders;
    
    std::vector<ValueKnob>      mKnobs;
    
    std::vector<float>          mMelodyAnims;
    std::vector<float>          mBassAnims;
    
    std::vector<WaveformButton> mWaveformButtons;
    Minim::Waveform* mWaveforms[WT_Count];
    float            mWaveformAnim;
    
    ofTrueTypeFont              mToggleFont;
    
    KeyChooser       mKeyChooser;
    ScaleChooser     mScaleChooser;
    
    ofImage          mTrebleClef;
    ofImage          mBassClef;

    Button           mButtonFx;
    
};


#endif
