#include "TPFont.h"
#include "TPCoordinate.h"

#define TPCOORD_MESH_X_COUNT  10
#define TPCOORD_MESH_Y_COUNT   6
#define TPCOORD_REFR_X_COUNT  10
#define TPCOORD_REFR_Y_COUNT   6
#define TPCOORD_REFR_MICROLH 100
#define TPCOORD_FONT_X_REFIX  0.014
#define TPCOORD_FONT_Y_REFIX  0.007

void TPCoordinate::SetDrawingPoints(TPCoord_RP_T type, float size, TPPoint* pts, unsigned szPts)
{
    mDrawingType = type;
    mDrawingSize = size;

    for (unsigned i = 0; i < szPts; ++i)
    {
        mDrawingPoints.push_back(TPPoint(pts[i]));
    }
}

void TPCoordinate::RenderPoints()
{
    glColor3f(0.0f, 1.0f, 1.0f);
    if (mDrawingType == TP_CURVE)
    {
        glLoadIdentity();
        if (mDrawingSize > 0.0 && mDrawingSize < 4.0)
            glLineWidth(mDrawingSize);
        glBegin(GL_LINE_STRIP);
        unsigned szPts = mDrawingPoints.size();
        for (unsigned i = 0; i < szPts; ++i)
        {
            glVertex3f(mDrawingPoints[i].x, mDrawingPoints[i].y, 0);
        }
        glEnd();
        glLineWidth(1.0);
    }

    if (mDrawingType == TP_POINT)
    {
        glLoadIdentity();
        if (mDrawingSize > 0.0 && mDrawingSize < 10.0)
            glPointSize(mDrawingSize);
        unsigned szPts = mDrawingPoints.size();
        for (unsigned i = 0; i < szPts; ++i)
        {
            glBegin(GL_POINTS);
            glVertex3f(mDrawingPoints[i].x, mDrawingPoints[i].y, 0);
            glEnd();
        }
        glPointSize(1.0);
    }
}

void TPCoordinate::RenderMesh()
{
    float rate, last;
    rate = (mView.GetPosX() - mView.GetNegX()) / (mMaxX - mMinX);
    mMeshStepX = (mMaxX - mMinX) / TPCOORD_MESH_X_COUNT * rate;
    last = mMinX + mMeshStepX;
    mMeshStartX = mMinX + mMeshStepX;
    for (float ix = mMinX + mMeshStepX; ix < mMaxX; ix += mMeshStepX)
    {
        glColor3f(0.0, 0.7, 0.0);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x1111);
        glBegin(GL_LINES);
        glVertex3f(ix, TP_MIN(mMinY,mView.GetNegY()), 0);
        glVertex3f(ix, TP_MAX(mMaxY,mView.GetPosY()), 0);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        last = ix;
        mMeshEndX = ix;
    }

    if (mView.GetNegX() < mMinX)
    {
        for (float ix = mMinX; ix > mView.GetNegX(); ix -= mMeshStepX)
        {
            glColor3f(0.0, 0.7, 0.0);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x1111);
            glBegin(GL_LINES);
            glVertex3f(ix, TP_MIN(mMinY, mView.GetNegY()), 0);
            glVertex3f(ix, TP_MAX(mMaxY, mView.GetPosY()), 0);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            mMeshStartX = ix;
        }
    }

    if (mView.GetPosX() > mMaxX)
    {
        for (float ix = last; ix < mView.GetPosX(); ix += mMeshStepX)
        {
            glColor3f(0.0, 0.7, 0.0);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x1111);
            glBegin(GL_LINES);
            glVertex3f(ix, TP_MIN(mMinY, mView.GetNegY()), 0);
            glVertex3f(ix, TP_MAX(mMaxY, mView.GetPosY()), 0);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            mMeshEndX = ix;
        }
    }

    mMeshStepY = (mMaxY - mMinY) / TPCOORD_MESH_Y_COUNT * rate;
    last = mMinY + mMeshStepY;
    mMeshStartY = mMinY + mMeshStepY;
    for (float iy = mMinY + mMeshStepY; iy < mMaxY; iy += mMeshStepY)
    {
        glColor3f(0.0, 0.7, 0.0);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x1111);
        glBegin(GL_LINES);
        glVertex3f(TP_MIN(mMinX, mView.GetNegX()), iy, 0);
        glVertex3f(TP_MAX(mMaxX, mView.GetPosX()), iy, 0);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        last = iy;
        mMeshEndY = iy;
    }
    
    if (mView.GetNegY() < mMinY)
    {
        for (float iy = mMinY; iy > mView.GetNegY(); iy -= mMeshStepY)
        {
            glColor3f(0.0, 0.7, 0.0);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x1111);
            glBegin(GL_LINES);
            glVertex3f(TP_MIN(mMinX, mView.GetNegX()), iy, 0);
            glVertex3f(TP_MAX(mMaxX, mView.GetPosX()), iy, 0);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            mMeshStartY = iy;
        }
    }

    if (mView.GetPosY() > mMaxY)
    {
        for (float iy = last; iy < mView.GetPosY(); iy += mMeshStepY)
        {
            glColor3f(0.0, 0.7, 0.0);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x1111);
            glBegin(GL_LINES);
            glVertex3f(TP_MIN(mMinX, mView.GetNegX()), iy, 0);
            glVertex3f(TP_MAX(mMaxX, mView.GetPosX()), iy, 0);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            mMeshEndY = iy;
        }
    }
}

void TPCoordinate::RenderReferenceValue()
{
    float step, microX = (mMaxX - mMinX) / TPCOORD_REFR_MICROLH, microY = microX;
    float fontReFixX = (mMaxX - mMinX) * TPCOORD_FONT_X_REFIX,
        fontReFixY = (mMaxY - mMinY) * TPCOORD_FONT_Y_REFIX;

    unsigned loopCount = 1;
    step = (mMaxX - mMinX) / TPCOORD_REFR_X_COUNT;
    for (float ix = mMinX + step; ix < mMaxX - step / 2; ix += step)
    {
        glBegin(GL_LINES);
        glVertex3f(ix, mMinY + microX, 0);
        glVertex3f(ix, mMinY, 0);
        glEnd();
        loopCount++;
    }

    float fontValueStepX = (mView.GetPosX() - mView.GetNegX()) / loopCount;
    loopCount = 1;
    for (float ix = mMinX + step; ix < mMaxX - step / 2; ix += step)
    {
        char value[16] = { 0 };
        sprintf(value, "%.2f", mView.GetNegX() + fontValueStepX * loopCount);
        TPDisplayString(value, ix - fontReFixX, mMinY + microY * 2);
        loopCount++;
    }

    loopCount = 1;
    step = (mMaxY - mMinY) / TPCOORD_REFR_Y_COUNT;
    for (float iy = mMinY + step; iy < mMaxY - step / 2; iy += step)
    {
        glBegin(GL_LINES);
        glVertex3f(mMinX + microY, iy, 0);
        glVertex3f(mMinX, iy, 0);
        glEnd();
        loopCount++;

    }

    float fontValueStepY = (mView.GetPosY() - mView.GetNegY()) / loopCount;
    loopCount = 1;
    for (float iy = mMinY + step; iy < mMaxY - step / 2; iy += step)
    {
        char value[16] = { 0 };
        sprintf(value, "%.2f", mView.GetNegY() + fontValueStepY * loopCount);
        TPDisplayString(value, mMinX + microY * 2, iy - fontReFixY);
        loopCount++;
    }
}