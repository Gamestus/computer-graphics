#pragma once
#include <Windows.h>



class DisplayWin32
{
public:
	HINSTANCE hInstance;
	int ClientWidth = 800;
	int ClientHeight = 600;
	HWND hWnd;
	WNDCLASSEX wc = { 0 };
	int Module; //?

	DisplayWin32(HINSTANCE hInstanceNew, LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam));
};

