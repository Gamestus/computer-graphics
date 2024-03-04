#include "Game.h"
#include <Windows.h>
#include <iostream>



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	
	Game game(hInstance);
	return game.Run();
}