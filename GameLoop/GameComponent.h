#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
#include "Game.h"

class GameComponent
{
public:
	Game* game;
	std::vector<std::unique_ptr<GameComponent>> children;

	Vector2 globalPosition;
	Vector2 localPosition;

	GameComponent();

	void DrawChildren();
	void UpdateChildren(float delta);
	/*virtual void DestroyResources();
	virtual void Reload();*/
private:
	void UpdatePosition(Vector2 parentGlobalPosition);
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

};

