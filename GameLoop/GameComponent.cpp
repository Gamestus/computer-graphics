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
void GameComponent::Update()
{
}

void GameComponent::DrawChildren() {
	Draw();
	for (auto& child : children) {
		child->DrawChildren();
	}
}

void GameComponent::UpdateChildren(float delta) {
	Update();
	for (auto& child : children) {
		child->UpdateChildren(delta);
	}
}

void GameComponent::UpdatePosition(Vector2 parentGlobalPosition)
{
	globalPosition = parentGlobalPosition + localPosition;
	for (auto& child : children) {
		child->UpdatePosition(globalPosition);
	}
}
