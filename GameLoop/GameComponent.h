#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
#include "Game.h"

class GameComponent
{
public:
	Game* game;
	std::vector<std::unique_ptr<GameComponent>> children;

	
	GameComponent();

	Vector2 GetGlobalPosition();
	Vector2 GetLocalPosition();

	void SetGlobalPosition(Vector2 pos);
	void SetLocalPosition(Vector2 pos);

	void DrawChildren();
	void UpdateChildren(float delta);
	/*virtual void DestroyResources();
	virtual void Reload();*/
protected:
	Vector2 globalPosition;
	Vector2 localPosition;
private:
	void UpdatePosition(Vector2 parentGlobalPosition);
	virtual void Initialize();
	virtual void Update(float delta);
	virtual void Draw();

};

