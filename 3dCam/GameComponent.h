#pragma once
#include <SimpleMath.h>
#include "Game.h"

using namespace DirectX::SimpleMath;

class GameComponent
{
public:
	Game* game;
	std::vector<std::unique_ptr<GameComponent>> children;

	
	GameComponent();

	void DrawChildren();
	void UpdateChildren(float delta);
	/*virtual void DestroyResources();
	virtual void Reload();*/
protected:
	Vector2 globalPosition;
	Vector2 localPosition;
private:
	virtual void Initialize();
	virtual void Update(float delta);
	virtual void Draw();

};

