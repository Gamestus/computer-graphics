#include "Game.h"
#include "DisplayWin32.h"

LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		wchar_t buffer[256];
		wsprintf(buffer, L"Key pressed: %d\n", static_cast<unsigned int>(wparam));
		OutputDebugStringW(buffer);

		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
		PostQuitMessage(0);
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

Game::Game(HINSTANCE hInstanceNew) {
	Initialize(hInstanceNew);
}

void Game::Initialize(HINSTANCE hInstanceNew) {
	Device = new DisplayWin32(hInstanceNew, MessageHandler);

}

Game::~Game() {
	delete[] Device;
}