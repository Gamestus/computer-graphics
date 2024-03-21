#pragma once
#include <SimpleMath.h>
#include "Game.h"

using namespace DirectX::SimpleMath;
namespace dx = DirectX;

class GameComponent
{
public:
	Game* game;
	GameComponent* parent;
	
	GameComponent();

	void DrawChildren();
	void UpdateChildren(float delta);

	template<typename T>
	T* AddChild(std::unique_ptr<GameComponent> child)
	{
		static_assert(std::is_base_of<GameComponent, T>::value,
			"T must be a GameComponent derived class!");

		child->parent = this;
		children.push_back(std::move(child));
		return dynamic_cast<T*>(children.back().get());
	}

	template<typename T>
	T* GetChild(int index)
	{
		return dynamic_cast<T*>(children[index].get());
	}
	virtual void Reparent(GameComponent* NewParent);
	virtual void Update(float delta);
	virtual void Draw();


protected:
	Vector2 globalPosition;
	Vector2 localPosition;
	std::vector<std::unique_ptr<GameComponent>> children;
private:
	virtual void Initialize();
};

