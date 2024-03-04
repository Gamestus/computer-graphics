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

Vector2 GameComponent::GetGlobalPosition()
{
	return globalPosition;
}

Vector2 GameComponent::GetLocalPosition()
{
	return localPosition;
}

void GameComponent::SetGlobalPosition(Vector2 pos) {
	globalPosition = pos;
	UpdatePosition(globalPosition); //TODO
}

void GameComponent::SetLocalPosition(Vector2 pos) {
	localPosition = pos;
	UpdatePosition(globalPosition); //TODO
}

void GameComponent::UpdateChildren(float delta) {
	Update(delta);
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
