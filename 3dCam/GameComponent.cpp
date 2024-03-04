#include "GameComponent.h"

GameComponent::GameComponent()
{
	Initialize();
}
void GameComponent::Draw()
{
}
void GameComponent::Initialize()
{
}
void GameComponent::Update(float delta)
{
}

void GameComponent::DrawChildren() {
	Draw();
	for (auto& child : children) {
		child->DrawChildren();
	}
}

void GameComponent::UpdateChildren(float delta) {
	Update(delta);
	for (auto& child : children) {
		child->UpdateChildren(delta);
	}
}

void GameComponent::AddChild(std::unique_ptr<GameComponent> child)
{
	child->parent.reset(this);
	children.push_back(std::move(child));
}
