#pragma once

#include "Game.h"

class GameComponent
{
public:
	Game* game;
	std::vector<GameComponent> children;


	GameComponent();

	void DrawChildren();
	void UpdateChildren(float delta);
	/*virtual void DestroyResources();
	virtual void Reload();*/
private:
	void Initialize();
	virtual void Update();
	virtual void Draw();

};

