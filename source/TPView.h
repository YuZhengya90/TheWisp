#pragma once

#include "TPConst.h"
#include "TPEnvironment.h"

class TPView
{
private:
    float mNegX;
    float mPosX;
    float mNegY;
    float mPosY;

    TPAnimation mTpAnimation;

public:
    TPView(float negX = 0.0, float posX = 0.0, float negY = 0.0, float posY = 0.0)
        : mNegX(negX), mPosX(posX), mNegY(negY), mPosY(posY)
    { }

    float GetNegX()const { return mNegX; } 
    float GetPosX()const { return mPosX; }
    float GetNegY()const { return mNegY; }
    float GetPosY()const { return mPosY; }

	bool DoingAnimation() const { return mTpAnimation.mDoing; }

    void SetAnchor(float minX, float maxX, float minY, float maxY){
        mNegX = minX;
        mPosX = maxX;
        mNegY = minY;
        mPosY = maxY;
    }

	void SetXAnchor(float minX, float maxX) {
		mNegX = minX;
		mPosX = maxX;
	}
	void SetYAnchor(float minY, float maxY) {
		mNegY = minY;
		mPosY = maxY;
	}

    void Translate(float x, float y);
    void Scale(TPPoint pts, float rate);
    void ScaleAnimation(TPPoint x, float rate, DWORD startTick, DWORD duringTime, unsigned frames);
    void Render();

private:

    void HandlingAnimation();
};