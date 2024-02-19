#pragma once
#include "DisplayWin32.h"
class Game
{
public:
	DisplayWin32 *Device;
	int backBuffer;
	int Context;
	int DebugAnnotation;
	int Instance;
	int Name;
	int PrevTime;
	int RenderSRV;
	int RenderView;
	int ScreenResized;
	int StartTime;
	int SwapChain;
	int TotalTime;

	Game(HINSTANCE hInstanceNew);
	~Game();

protected:
	void Initialize(HINSTANCE hInstanceNew);
};

