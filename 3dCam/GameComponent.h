#pragma once
#include <SimpleMath.h>
#include "Game.h"

using namespace DirectX::SimpleMath;

class GameComponent
{
public:
	Game* game;
	std::unique_ptr<GameComponent> parent;
	
	GameComponent();

	void DrawChildren();
	void UpdateChildren(float delta);
	virtual void AddChild(std::unique_ptr<GameComponent> child);

	template<typename T>
	T* GetChild(int index)
	{
		return dynamic_cast<T*>(children[index].get());
	}
	virtual void Update(float delta);
	virtual void Draw();
protected:
	Vector2 globalPosition;
	Vector2 localPosition;
	std::vector<std::unique_ptr<GameComponent>> children;
private:
	virtual void Initialize();
};

