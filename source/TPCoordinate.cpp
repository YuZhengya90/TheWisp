#include <algorithm>
#include "TPFont.h"
#include "TPCoordinate.h"

#define TPCOORD_MESH_X_COUNT  10
#define TPCOORD_MESH_Y_COUNT   6
#define TPCOORD_REFR_X_COUNT  10
#define TPCOORD_REFR_Y_COUNT   6
#define TPCOORD_REFR_MICROLH 100
#define TPCOORD_FONT_X_REFIX  0.012
#define TPCOORD_FONT_Y_REFIX  0.007
#define TPCOORD_DATE_X_MIN    30
#define TPCOORD_VALU_Y_MIN     5

TPCoordinate::TPCoordinate(char * name)
	:mName(name), mMinX(0), mMaxX(10), mMinY(0), mMaxY(10), mYType(XY_FLOAT)
{
	Init();
}

void TPCoordinate::Init()
{
	mHoveredPoint = -1;
	mEnableFeatures = 0;
}

void TPCoordinate::SetXAnchor(TPDate minX, TPDate maxX)
{
	mMinX = (float)minX.ToInt();
	mMaxX = (float)maxX.ToInt();
	mView.SetXAnchor(mMinX, mMaxX);
}

void TPCoordinate::SetXAnchor(int minX, int maxX)
{	
	mMinX = (float)minX;
	mMaxX = (float)maxX;
	mView.SetXAnchor(mMinX, mMaxX);
}

void TPCoordinate::SetYAnchor(double minY, double maxY)
{
	mYType = XY_FLOAT;
	mMinY = (float)minY;
	mMaxY = (float)maxY;
	mView.SetYAnchor(mMinY, mMaxY);
}

void TPCoordinate::SetYAnchor(int minY, int maxY)
{
	mYType = XY_INT;
	mMinY = (float)minY;
	mMaxY = (float)maxY;
	mView.SetYAnchor(mMinY, mMaxY);
}

void TPCoordinate::SetValues(std::vector<TPDate> dates, std::vector<double> values)
{
	std::vector<string> titles;
	unsigned szDates = dates.size();
	for (unsigned i = 0; i < szDates; ++i)
	{
		titles.push_back(dates[i].ToString());
	}

	SetValues(dates, values, titles, false);
}

void TPCoordinate::SetValues(std::vector<TPDate> dates, std::vector<double> values, std::vector<string> titles, bool onlyTable)
{
	Init();

	unsigned dateSize = dates.size();
	unsigned valueSize = values.size();
	unsigned titleSize = titles.size();

	if (titleSize != valueSize)
	{
		return;
	}

	mTableFromto = dates;
	mTableTitles = titles;
	mTableValues = values;

	SetXAnchor(0, ILLU_W);
	SetYAnchor(0, ILLU_H);
	mTableAnchor.Push(mMinX, mMaxX, mMinY, mMaxY, mView);

	if (!onlyTable)
	{
		if (dateSize < TPCOORD_DATE_X_MIN * 2)
		{
			int requiredDataSize = TPCOORD_DATE_X_MIN * 2 - dateSize;
			SetXAnchor(dates[0] - requiredDataSize / 2, dates[dateSize - 1] + requiredDataSize / 2);
		}
		else
		{
			SetXAnchor(dates[0], dates[dateSize - 1]);
		}		
		
		auto pairMinMax = std::minmax_element(values.begin(), values.end());
		double duration = *pairMinMax.second - *pairMinMax.first;
		if (FEZ(duration))
		{
			SetYAnchor(*pairMinMax.first - TPCOORD_VALU_Y_MIN, *pairMinMax.second + TPCOORD_VALU_Y_MIN);
		}
		else
		{
			SetYAnchor(*pairMinMax.first - 0.40 * duration, *pairMinMax.second + 0.40 * duration);
		}
		
		mIllusionAnchor.Push(mMinX, mMaxX, mMinY, mMaxY, mView);
		SetEnableFeatures(F_MESH, true);
		SetEnableFeatures(F_CROSSLINE, true);
		SetEnableFeatures(F_POINT, true);
		SetEnableFeatures(F_TABLE, false);
	}
	else
	{
		SetEnableFeatures(F_TABLE, true);
	}
}

void TPCoordinate::RenderPoints()
{
    glColor3f(0.0f, 1.0f, 1.0f);
    if (IsEnableFeatures(F_CURVE))
    {
        glLoadIdentity();
        glBegin(GL_LINE_STRIP);
        unsigned szPts = mTableFromto.size();
        for (unsigned i = 0; i < szPts; ++i)
        {
			glVertex3f(mTableFromto[i].ToInt(), mTableValues[i], 0);
        }
        glEnd();
    }

	if (IsEnableFeatures(F_POINT))
    {
        glLoadIdentity();
		unsigned szPts = mTableFromto.size();
		float disX = 1.0f, disY = 1.0f;
		if (szPts > 1)
		{
			disX = 0.4;
			disY = (mMaxY - mMinY) / (mMaxX - mMinX) * 0.6;
			for (unsigned i = 0; i < szPts; ++i)
			{
				if (mHoveredPoint == i)
				{
					glColor3f(0.6f, 1.0f, 0.6f);
				}
				else
				{
					glColor3f(0.0f, 1.0f, 1.0f);
				}
				
				glBegin(GL_QUADS);
				glVertex3f(mTableFromto[i].ToInt() - disX, mTableValues[i] - disY, 0);
				glVertex3f(mTableFromto[i].ToInt() - disX, mTableValues[i] + disY, 0);
				glVertex3f(mTableFromto[i].ToInt() + disX, mTableValues[i] + disY, 0);
				glVertex3f(mTableFromto[i].ToInt() + disX, mTableValues[i] - disY, 0);
				glEnd();

				glColor3f(0.5, 0.5, 0.5);
				glBegin(GL_LINE_STRIP);
				glVertex3f(mTableFromto[i].ToInt() - disX, mTableValues[i] - disY, 0);
				glVertex3f(mTableFromto[i].ToInt() - disX, mTableValues[i] + disY, 0);
				glVertex3f(mTableFromto[i].ToInt() + disX, mTableValues[i] + disY, 0);
				glVertex3f(mTableFromto[i].ToInt() + disX, mTableValues[i] - disY, 0);
				glVertex3f(mTableFromto[i].ToInt() - disX, mTableValues[i] - disY, 0);
				glEnd();
			}
		}

        glPointSize(1.0);
    }
}

void TPCoordinate::RenderMesh()
{
    float rate, last;
    rate = (mView.GetPosX() - mView.GetNegX()) / (mMaxX - mMinX);
    float mMeshStepX = (mMaxX - mMinX) / TPCOORD_MESH_X_COUNT * rate;
    last = mMinX + mMeshStepX;
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
        }
    }

    float mMeshStepY = (mMaxY - mMinY) / TPCOORD_MESH_Y_COUNT * rate;
    last = mMinY + mMeshStepY;
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
        }
    }
}

void TPCoordinate::RenderCrossLine()
{
	if (mView.DoingAnimation())
	{
		return;
	}

	unsigned szPts = mTableFromto.size();
	for (unsigned i = 0; i < szPts; ++i)
	{
		if (mHoveredPoint == i)
		{
			float fontReFixX = (mView.GetPosX() - mView.GetNegX()) * TPCOORD_FONT_X_REFIX * 2;
			float fontReFixY = (mView.GetPosY() - mView.GetNegY()) * TPCOORD_FONT_Y_REFIX;

			glColor3f(0.8, 0.3, 0.3);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0x1F1F);
			glBegin(GL_LINES);
			glVertex3f(mView.GetNegX() - fontReFixX, mTableValues[i], 0);
			glVertex3f(mView.GetPosX() - fontReFixX, mTableValues[i], 0);
			glEnd();
			glDisable(GL_LINE_STIPPLE);

			char value[16] = { 0 };
			if (mYType == XY_INT)
			{
				sprintf_s(value, "%d", (int)roundf(mTableValues[i]));
				TPDisplayString(value, mView.GetPosX() - fontReFixX, 
					TP_MAX(mTableValues[i] - fontReFixY, mView.GetNegY() + fontReFixY * 0.2));
			}

			else
			{
				sprintf_s(value, "%.2f", (float)mTableValues[i]);
				TPDisplayString(value, mView.GetPosX() - fontReFixX, 
					TP_MAX(mTableValues[i] - fontReFixY, mView.GetNegY() + fontReFixY * 0.2));
			}

			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0x1F1F);
			glBegin(GL_LINES);
			glVertex3f(mTableFromto[i].ToInt(), mView.GetNegY() - 4 * fontReFixY, 0);
			glVertex3f(mTableFromto[i].ToInt(), mView.GetPosY() - 4 * fontReFixY, 0);
			glEnd();
			glDisable(GL_LINE_STIPPLE);


			TPDisplayString(mTableFromto[i].ToString().c_str(), 
				TP_MAX((mTableFromto[i].ToInt() - fontReFixX), mView.GetNegX() + fontReFixX * 0.2)
				, mView.GetPosY() - 3 * fontReFixY);
		}
	}
}

void TPCoordinate::RenderReferenceValue()
{
	float step;
	float microX = (mMaxY - mMinY) / TPCOORD_REFR_MICROLH;
	float microY = (mMaxX - mMinX) / TPCOORD_REFR_MICROLH;
	float fontReFixX = (mMaxX - mMinX) * TPCOORD_FONT_X_REFIX;
    float fontReFixY = (mMaxY - mMinY) * TPCOORD_FONT_Y_REFIX;

    unsigned loopCount = 1;
	step = GetSuitableXStep();
	if (step > 0.0f)
	{
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
			float curValue = mView.GetNegX() + fontValueStepX * loopCount;
			TPDate curDate = TPDate::FromInt((int)roundf(curValue));
			TPDisplayString(curDate.ToString().c_str(), ix - 2 * fontReFixX, mMinY + microX * 2);
			loopCount++;
		}
	}

    loopCount = 1;
	step = GetSuitableYStep();
	if (step > 0.0f)
	{
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
			float curValue = mView.GetNegY() + fontValueStepY * loopCount;

			if (mYType == XY_INT)
			{
				sprintf_s(value, "%d", (int)roundf(curValue));
				TPDisplayString(value, mMinX + microY * 2, iy - fontReFixY);
			}

			else
			{
				sprintf_s(value, "%.2f", curValue);
				TPDisplayString(value, mMinX + microY * 2, iy - fontReFixY);
			}

			loopCount++;
		}
	}
}

void TPCoordinate::RenderTables()
{
	TPDisplayString2(mName, 50, 500, 40, 1);
	std::string dateString = mTableFromto[0].ToString();
	if (mTableFromto.size() >= 1)
	{
		dateString = mTableFromto[0].ToString() + "-" + mTableFromto[mTableFromto.size() - 1].ToString();
	}
	
	TPDisplayString2(dateString.c_str(), 50, 450, 20, 0);

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex3f(50.0f, 420.0, 0);
	glVertex3f(1190.0f, 420.0, 0);
	glEnd();
	glLineWidth(1);

	unsigned tableTitleSize = mTableTitles.size();
	unsigned tableValueSize = mTableValues.size();

	if (tableTitleSize != tableValueSize)
	{
		return;
	}

	for (unsigned i = 0; i < tableTitleSize; ++i)
	{
		int yp = (320 - i * 80);
		TPDisplayString2(mTableTitles[i].c_str(), 50, I2F(yp), 40, 1);

		char value[16] = { 0 };
		sprintf_s(value, "%.2lf", mTableValues[i]);
		TPDisplayString2(value, 750, I2F(yp), 40, 0);
	}
}

void TPCoordinate::SetEnableFeatures(TPCoord_F_T efType, bool enable)
{
	if (efType == F_TABLE)
	{
		if (IsEnableFeatures(efType) != enable)
		{
			if (enable)
			{
				Exchange2TableAnchor();
			}
			else
			{
				Exchange2IllusionAnchor();
			}
		}
	}

	if (enable)
	{
		mEnableFeatures |= (int)efType;
	}
	else
	{
		mEnableFeatures &= (~(int)efType);
	}
}

bool TPCoordinate::IsEnableFeatures(TPCoord_F_T efType)
{
	return (mEnableFeatures & efType) != 0;
}

float TPCoordinate::GetSuitableXStep()
{
	int duration = (int)roundf((mMaxX - mMinX));
	const int delta[] = { 5, 6, 8, 10 };
	const int deltaSize = sizeof(delta) / sizeof(delta[0]);
	while (true)
	{
		for (int i = 0; i < deltaSize; ++i)
		{
			if (duration % delta[i] == 0)
			{
				if ((int)roundf(mMaxX - mMinX) >= delta[i])
				{
					return (int)roundf(mMaxX - mMinX) / delta[i];
				}
					
				return -1;
			}
		}
		duration++;
	}

	return (mMaxX - mMinX) / TPCOORD_REFR_X_COUNT;	
}

float TPCoordinate::GetSuitableYStep()
{
	if (mYType == XY_INT)
	{
		int duration = (int)(mMaxY - mMinY);
		const int delta[] = { 2, 3, 4, 5, 6 };
		const int deltaSize = sizeof(delta) / sizeof(delta[0]);
		while (true)
		{
			for (int i = 0; i < deltaSize; ++i)
			{
				if (duration % delta[i] == 0)
				{
					if ((int)roundf(mMaxY - mMinY) >= delta[i])
					{
						return (int)roundf(mMaxY - mMinY) / delta[i];
					}
					return -1;
				}
			}
			duration++;
		}
	}

	return (mMaxY - mMinY) / TPCOORD_REFR_Y_COUNT;
}

int TPCoordinate::HoverPoint(TPPoint illuP)
{
	unsigned szPts = mTableFromto.size();
	float disX = 0.4;
	float disY = (mMaxY - mMinY) / (mMaxX - mMinX) * 0.6;
	
	for (unsigned i = 0; i < szPts; ++i)
	{
		if (illuP.x > mTableFromto[i].ToInt() - disX && illuP.x < mTableFromto[i].ToInt() + disX
			&& illuP.y > mTableValues[i] - disY && illuP.y < mTableValues[i] + disY)
		{
			mHoveredPoint = i;
			return i;
		}
	}

	mHoveredPoint = -1;
	return -1;
}

void TPCoordinate::Exchange2TableAnchor()
{
	mIllusionAnchor.Push(mMinX, mMaxX, mMinY, mMaxY, mView);
	mTableAnchor.Pop(mMinX, mMaxX, mMinY, mMaxY, mView);
}

void TPCoordinate::Exchange2IllusionAnchor()
{
	mTableAnchor.Push(mMinX, mMaxX, mMinY, mMaxY, mView);
	mIllusionAnchor.Pop(mMinX, mMaxX, mMinY, mMaxY, mView);
}