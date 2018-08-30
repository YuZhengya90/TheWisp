#pragma once

#include <vector>
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

class TPDate
{
public:
	TPDate();
	TPDate(int year, int month, int day);
	TPDate(int date); // "20180101" "yyyymmdd"
	TPDate(const TPDate& o);
	TPDate& operator=(const TPDate& o);
	
	bool operator<(TPDate o);
	bool operator<=(TPDate o);
	bool operator>(TPDate o);
	bool operator>=(TPDate o);
	int operator-(TPDate o);
	TPDate operator+(int days);
	TPDate operator-(int days);
	TPDate& operator++();
	TPDate& operator++(int);
	TPDate& operator--();
	TPDate& operator--(int);

	operator float();
	operator int();

	// "20180101"
	std::string ToString();
	// days from 20000101
	int ToInt();

	// daysTill2000 means days from 20000101
	static TPDate FromInt(int daysTill2000);
	static std::vector<TPDate> GetVector(TPDate sDate, TPDate eDate);

	int GetYear();
	int GetMonth();
	int GetDay();


private:

	bool IsLeap();
	static bool IsLeap(int year);

	int mY;
	int mM;
	int mD;
};


class TPBitmap
{
public:
	TPBitmap();
	~TPBitmap();

	bool Load(const char* path);
	void Display(float x, float y);

private:

	void LoadImageFile(const char* imageName);

	struct ImageFileDate {
		unsigned long bWidth;
		unsigned long bHeight;
		unsigned char* fileData;

	};

	struct ImageFileDate *mImage;
	GLuint texture[1];

};