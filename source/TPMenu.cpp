#include "TPMenu.h"

void TPInitMenu(HINSTANCE hInstance, HWND hwnd)
{
	// Sample 

	HWND purPreBtn = CreateWindow(
		"BUTTON", 
		"Purchase Predict Sample", 
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		30, 30, 180, 30, 
		hwnd, 
		(HMENU)PUR_PREDICT_BTN, 
		hInstance, 
		NULL);

	HWND salPreBtn = CreateWindow(
		"BUTTON",
		"Sales Predict Sample",
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		30, 60, 180, 30,
		hwnd,
		(HMENU)SAL_PREDICT_BTN,
		hInstance,
		NULL);
}