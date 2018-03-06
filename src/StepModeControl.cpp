#include "StepModeControl.h"
#include "Params.h"
#include "Melodizer.h"

StepModeControl::StepModeControl(IPlugBase* pPlug, IRECT iRect, const int paramIdx, IText* textStyle)
: IControl(pPlug, iRect, paramIdx)
{
	SetText(textStyle);
	mNormRect = iRect.SubRectVertical(3, 2).GetPadded(0, 1, 0, 0);
	mSkipRect = iRect.SubRectVertical(3, 1);
	mLoopRect = iRect.SubRectVertical(3, 0).GetPadded(0, 0, 0, -1);

	// expand rect to the left to account for the labels we will draw
	mRECT.L -= 20;
	mTargetRECT = mRECT;
}

bool StepModeControl::Draw(IGraphics *pGraphics)
{
	//pGraphics->DrawRect(&mText.mTextEntryFGColor, &mRECT);
	IColor* normColor = &mText.mTextEntryBGColor;
	IColor* skipColor = &mText.mTextEntryBGColor;
	IColor* loopColor = &mText.mTextEntryBGColor;
	switch(GetParam()->Int())
	{
		case SM_Norm: normColor = &mText.mTextEntryFGColor; break;
		case SM_Skip: skipColor = &mText.mTextEntryFGColor; break;
		case SM_Loop: loopColor = &mText.mTextEntryFGColor; break;
	}
	pGraphics->FillIRect(normColor, &mNormRect);
	pGraphics->FillIRect(skipColor, &mSkipRect);
	pGraphics->FillIRect(loopColor, &mLoopRect);
	
	mText.mAlign = IText::kAlignFar;
	const int pad = -mNormRect.W()-2;
	IRECT textRect = mNormRect.GetPadded(pad, 0, pad, 0);
	pGraphics->DrawIText(&mText, "NORM", &textRect);
	
	textRect = mSkipRect.GetPadded(pad, 0, pad, 0);
	pGraphics->DrawIText(&mText, "SKIP", &textRect);
	
	textRect = mLoopRect.GetPadded(pad, 0, pad, 0);
	pGraphics->DrawIText(&mText, "LOOP", &textRect);
	
	return true;
}

void StepModeControl::OnMouseDown(int x, int y, IMouseMod *mod)
{
	if (mod->L)
	{
		if (mNormRect.Contains(x, y))
		{
			SetValueFromUserInput(GetParam()->GetNormalized(SM_Norm));
		}
		else if (mSkipRect.Contains(x, y))
		{
			SetValueFromUserInput(GetParam()->GetNormalized(SM_Skip));
		}
		else if (mLoopRect.Contains(x, y))
		{
			SetValueFromUserInput(GetParam()->GetNormalized(SM_Loop));
		}
	}
	else if (mod->R)
	{
		Melodizer* plug = static_cast<Melodizer*>(mPlug);
		if (plug != nullptr)
		{
			plug->BeginMIDILearn(mParamIdx, -1, x, y);
		}
	}
}
