#include "GameComponent.h"
#include <cassert>

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
void GameComponent::Reparent(GameComponent* NewParent)
{
    assert(parent != nullptr);
    assert(NewParent != nullptr);
    assert(parent->children.size() > 0);

    auto it = std::find_if(parent->children.begin(), parent->children.end(),
        [this](const std::unique_ptr<GameComponent>& child) {
            return child.get() == this;
        });

    if (it != parent->children.end())
    {
        std::unique_ptr<GameComponent> child = std::move(*it); 
        parent->children.erase(it);
        NewParent->children.push_back(std::move(child));
    }

    parent = NewParent;
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