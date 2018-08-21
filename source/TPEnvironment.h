#pragma once

#include "TPConst.h"

class TPPoint
{
public:

	float x;
	float y;

	TPPoint(float a = 0.0, float b = 0.0)
		: x(a), y(b) { }

	TPPoint operator-(const TPPoint& p);
	TPPoint operator+(const TPPoint& p);
};

class TPAnimation
{
public:

    TPPoint mPts;
    float mRate;
    DWORD mStartTick;
    DWORD mDuringTime;
    unsigned mFrames;
    unsigned mNowFrame;
    bool mDoing;

    TPAnimation() : mDoing(false){ }
};