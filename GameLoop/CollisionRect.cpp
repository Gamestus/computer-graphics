#include "CollisionRect.h"

CollisionRect::CollisionRect()
{
	Initialize();
}

CollisionRect::CollisionRect(Vector2 size)
{
	rect2D = SimpleMath::Rectangle(0, 0, size.x, size.y);
	PhysicsServer::Instance->RegisterRect(this);
}

CollisionRect::CollisionRect(Vector2 size, bool isHorizontal)
{
	IsHorizontal = isHorizontal;
	rect2D = SimpleMath::Rectangle(0, 0, size.x, size.y);
	PhysicsServer::Instance->RegisterRect(this);
}

CollisionRect::CollisionRect(CollisionCallback callback) : callback_(callback)
{
	Initialize();
}

void CollisionRect::Initialize()
{
	rect2D = SimpleMath::Rectangle(0, 0, 200, 200);
	PhysicsServer::Instance->RegisterRect(this);
}


void CollisionRect::Update(float delta)
{
	rect2D.x = (float)globalPosition.x * Game::Instance->Display->ClientWidth / 2;
	rect2D.y = -(float)globalPosition.y * Game::Instance->Display->ClientHeight / 2;
}