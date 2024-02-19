#include "Game.h"
#include <Windows.h>
#include <iostream>



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	
	Game game(hInstance);



	BOOL gResult;
	MSG msg = {};
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1) {
		return -1;
	}
	else {
		return msg.wParam;
	}
}