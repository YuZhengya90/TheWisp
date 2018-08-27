#include "stdafx.h"
#include "TPEnvironment.h"

TPPoint TPPoint::operator - (const TPPoint& p)
{
	return TPPoint(x - p.x, y - p.y);
}

TPPoint TPPoint::operator + (const TPPoint& p)
{
	return TPPoint(x + p.x, y + p.y);
}

TPDate::TPDate()
	:mY(2000), mM(1), mD(1)
{
}

TPDate::TPDate(int year, int month, int day)
	: mY(year), mM(month), mD(day)
{
}

TPDate::TPDate(int date)
{
	mY = date / 10000;
	mM = (date - mY * 10000) / 100;
	mD = date - mY * 10000 - mM * 100;
}

TPDate::TPDate(const TPDate& o)
	:mY(o.mY), mM(o.mM), mD(o.mD)
{
}

TPDate& TPDate::operator=(const TPDate& o)
{
	mY = o.mY; mM = o.mM; mD = o.mD;
	return *this;
}

bool TPDate::operator<(TPDate o)
{
	return ToInt() < o.ToInt();
}

bool TPDate::operator<=(TPDate o)
{
	return ToInt() <= o.ToInt();
}

bool TPDate::operator>(TPDate o)
{
	return !(operator<=(o));
}

bool TPDate::operator>=(TPDate o)
{
	return !(operator>(o));
}

int TPDate::operator-(TPDate o)
{
	return ToInt() - o.ToInt();
}

TPDate TPDate::operator + (int days)
{
	return FromInt(ToInt() + days);
}

TPDate TPDate::operator-(int days)
{
	return FromInt(ToInt() - days);
}

TPDate& TPDate::operator++()
{
	*this = FromInt(ToInt() + 1);
	return *this;
}

TPDate& TPDate::operator++(int)
{
	*this = FromInt(ToInt() + 1);
	return *this;
}

TPDate& TPDate::operator--()
{
	*this = FromInt(ToInt() - 1);
	return *this;
}

TPDate& TPDate::operator--(int)
{
	*this = FromInt(ToInt() - 1);
	return *this;
}

TPDate::operator float()
{
	return (float)ToInt();
}

TPDate::operator int()
{
	return ToInt();
}

std::string TPDate::ToString()
{
	char str[16] = { 0 };
	_itoa_s(10000 * mY + 100 * mM + mD, str, 10);
	return std::string(str);
}

int TPDate::ToInt()
{
	const int dayTill[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	int days = 365 * (mY - 2000) + dayTill[(mM - 1)] + (mD - 1);
	int leapCount = 0;
	for (int x = 2000; x < mY; ++x)
	{
		if (TPDate::IsLeap(x))
		{
			leapCount++;
		}
	}

	if (TPDate::IsLeap(mY) && mM > 2)
	{
		leapCount++;
	}

	return days + leapCount;
}

TPDate TPDate::FromInt(int x)
{
	int accDay = 0;
	const int dayTill[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const int dayTillLeap[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
	int y, m, d;
	for (int i = 0; i < 20; ++i)
	{
		int yF = 365;
		if (TPDate::IsLeap(i + 2000))
		{
			yF = 366;
		}

		int remained = x - accDay;
		if (remained < yF)
		{
			remained += 1;
			y = 2000 + i;
			for (int ii = 0; ii < 12; ++ii)
			{
				if (yF == 366)
				{
					if (dayTillLeap[ii] - remained < 0 && dayTillLeap[ii + 1] - remained >= 0)
					{
						m = ii + 1;
						d = remained - dayTillLeap[ii];
						return TPDate(y, m, d);
					}
				}

				else
				{
					if (dayTill[ii] - remained < 0 && dayTill[ii + 1] - remained >= 0)
					{
						m = ii + 1;
						d = remained - dayTill[ii];
						return TPDate(y, m, d);
					}
				}
			}
		}

		accDay += yF;
	}

	return TPDate(2000, 1, 1);
}

std::vector<TPDate> TPDate::GetVector(TPDate sDate, TPDate eDate)
{
	std::vector<TPDate> ret;
	if (eDate < sDate)
	{
		return ret;
	}

	for (TPDate i = sDate; i <= eDate; ++i)
	{
		ret.push_back(i);
	}

	return ret;
}

bool TPDate::IsLeap()
{
	// only support 2000~2020 right now.
	return GetYear() == 2000
		|| GetYear() == 2004
		|| GetYear() == 2008
		|| GetYear() == 2012
		|| GetYear() == 2016
		|| GetYear() == 2020;
}

bool TPDate::IsLeap(int year)
{
	return TPDate(year, 1, 1).IsLeap();	
}

int TPDate::GetYear()
{
	return mY;
}

int TPDate::GetMonth()
{
	return mM;
}

int TPDate::GetDay()
{
	return mD;
}