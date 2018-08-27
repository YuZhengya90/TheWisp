#include "stdafx.h"
#include "TPView.h"

#define REFVALUE_XCOUNT  10
#define REFVALUE_YCOUNT   6
#define REFVALUE_MICROL 100

void TPView::Translate(float x, float y)
{
    mNegX += x;
    mPosX += x;
    mNegY += y;
    mPosY += y;
}

void TPView::Scale(TPPoint pts, float rate)
{
    mNegX = pts.x - (pts.x - mNegX) * rate;
    mPosX = pts.x - (pts.x - mPosX) * rate;
    mNegY = pts.y - (pts.y - mNegY) * rate;
    mPosY = pts.y - (pts.y - mPosY) * rate;
}

void TPView::ScaleAnimation(TPPoint pts, float rate, DWORD startTick, DWORD duringTime, unsigned frames)
{
    mTpAnimation.mPts = pts;
    mTpAnimation.mRate = rate;
    mTpAnimation.mStartTick = startTick;
    mTpAnimation.mDuringTime = duringTime;
    mTpAnimation.mFrames = frames;
    mTpAnimation.mNowFrame = 0;
    mTpAnimation.mDoing = true;
}

void TPView::Render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mNegX, mPosX, mNegY, mPosY, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(MENU_W + 3 * SCRN_B, SCRN_B, ILLU_W, ILLU_H);

    HandlingAnimation();
}

void TPView::HandlingAnimation()
{
    if (!mTpAnimation.mDoing)
    {
        return;
    }

    DWORD tickDistance = (GetTickCount() - mTpAnimation.mStartTick);
    DWORD tickInternal = (mTpAnimation.mDuringTime / mTpAnimation.mFrames);
    if ( tickDistance > mTpAnimation.mNowFrame * tickInternal)
    {
        Scale(mTpAnimation.mPts, mTpAnimation.mRate);
        mTpAnimation.mNowFrame += 1;
    }

    if (tickDistance > mTpAnimation.mDuringTime)
    {
        mTpAnimation.mDoing = false;
    }
}