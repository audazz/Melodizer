//
//  KeyChooser.cpp
//  melodizer
//
//  Created by Damien Di Fede on 12/14/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "KeyChooser.h"
#include "Settings.h"
#include <math.h>
#include <cassert>

// display text for keys
static const char * keys[] = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };

// length of open/close anim
static const float  sk_AnimLength = 0.15f;

//--------------------------------
KeyChooser::KeyChooser()
: mBox(0,0,0,0)
, mRadius( 0 )
, mState(ST_CLOSED)
, mAnim( 0 )
{
}

//--------------------------------
KeyChooser::~KeyChooser()
{
    for( int i = 0; i < 12; ++i )
    {
        delete mKeys[i];
    }
}

//--------------------------------
void KeyChooser::setup( const float cx, const float cy, const float buttonDim, const float radius )
{
    mBox.mW = buttonDim;
    mBox.mH = buttonDim;
    mBox.setCenter(cx, cy);
    mRadius = radius;
    for( int i = 0; i < 12; ++i )
    {
        const float r = (i%2)==0 ? mRadius : mRadius*0.75f;
        const float x = r * sinf( i * M_PI_2/11 );
        const float y = r * -cosf( i * M_PI_2/11 );
        const int   k = i;
        mKeys[i] = new KeyButton( keys[k], x, y, buttonDim );
    }
    
    assert( mFont.loadFont("HelveticaBold.ttf", 32 *(ofGetWidth()/1024.0f)) && "Failed to load KeyChooser font!" );
}

//--------------------------------
void KeyChooser::open()
{
    mState = ST_OPENING;
    mAnim = sk_AnimLength;
}

//--------------------------------
void KeyChooser::close()
{
    mState = ST_CLOSING;
    mAnim  = sk_AnimLength;
}

//--------------------------------
bool KeyChooser::active()
{
    return mState != ST_CLOSED;
}

//--------------------------------
bool KeyChooser::handleTouch( const float x, const float y )
{
    if ( mState == ST_OPEN )
    {
        // see if they touched a key
        for( int i = 0; i < 12; ++i )
        {
            if ( mKeys[i]->mBox.contains( x, y ) )
            {
                // set the new key and close
                Settings::Key = i;
                break;
            }
        }
        close();
    }
    else if ( mState == ST_CLOSED && mBox.contains( x, y ) )
    {
        open();
    }
    
    return active();
}

//--------------------------------
void KeyChooser::update( const float dt )
{
    if ( mAnim > 0 )
    {
        mAnim -= dt;
        if ( mAnim < 0 )
        {
            mAnim = 0;
            
            if ( mState == ST_OPENING )
            {
                mState = ST_OPEN;
            }
            else if ( mState == ST_CLOSING )
            {
                mState = ST_CLOSED;
            }
        }
    }
}

//--------------------------------
void KeyChooser::drawButton( KeyButton* button, float alpha )
{
    Box& b = button->mBox;
 
    ofFill();
    
    if ( keys[Settings::Key] == button->mName )
    {
        ofSetColor(255, 128, 0, alpha*0.95f);
    }
    else
    {
        ofSetColor(80, 80, 80, alpha*0.95f);
    }
    ofRect( b.mX, b.mY, b.mW, b.mH );
    
    ofSetColor(200, 200, 200, alpha);
    const float yoff = mFont.getLineHeight() * 0.35f;
    const float xoff = mFont.stringWidth( button->mName ) * -0.5f;
    mFont.drawString( button->mName, b.mX + xoff, b.mY + yoff );
}

//--------------------------------
void KeyChooser::draw()
{
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    switch ( mState ) 
    {
        case ST_OPENING:
        {
            const float posScale = 1.f - mAnim/sk_AnimLength;
            
            ofSetColor(0, 0, 0, 200.0f * posScale);
            ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
            
            for( int i = 0; i < 12; ++i )
            {
                mKeys[i]->mBox.setCenter(mBox.mX + posScale*mKeys[i]->mOpenX, mBox.mY + posScale*mKeys[i]->mOpenY);
                drawButton( mKeys[i], 255 * posScale );
            }
        }
        break;
            
        case ST_OPEN:
        {
            ofSetColor(0, 0, 0, 200);
            ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
            
            for( int i = 0; i < 12; ++i )
            {
                mKeys[i]->mBox.setCenter(mBox.mX + mKeys[i]->mOpenX, mBox.mY + mKeys[i]->mOpenY);
                drawButton( mKeys[i], 255 );
            }
        }
        break;
            
        case ST_CLOSING:
        {
            const float posScale = mAnim/sk_AnimLength;
            
            ofSetColor(0, 0, 0, 200.0f * posScale);
            ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
            
            for( int i = 0; i < 12; ++i )
            {
                mKeys[i]->mBox.setCenter(mBox.mX + posScale*mKeys[i]->mOpenX, mBox.mY + posScale*mKeys[i]->mOpenY);
                drawButton( mKeys[i], 255 * (1 - posScale) );
            }
        }
        break;
            
        case ST_CLOSED:
        {
            const int k = Settings::Key;
            mKeys[k]->mBox.setCenter( mBox.mX, mBox.mY );
            drawButton( mKeys[k], 255 );
        }
        break;
            
            
        default:
            break;
    }
}
