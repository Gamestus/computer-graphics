#pragma once
#include "DisplayWin32.h"
#include "InputDevice.h"
#include <optional>

class Game
{
public:
	static Game* Instance;
	DisplayWin32* Display;
	InputDevice* Device;
	int backBuffer;
	int Context;
	int DebugAnnotation;
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

	int Run();

private:
	std::optional<int> processMessages();
	void DoFrame();
protected:
	void Initialize(HINSTANCE hInstanceNew);
};

