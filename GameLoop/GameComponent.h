#pragma once

#include "Game.h"

class GameComponent
{
public:
	Game* game;

	GameComponent();
	void Draw();
	/*virtual void DestroyResources();
	void Initialize();
	virtual void Reload();
	virtual void Update(); */
};

