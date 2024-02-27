#include "CollisionRect.h"

CollisionRect::CollisionRect()
{
	Initialize();
}

//CollisionRect::CollisionRect(SimpleMath::Rectangle rect)
//{
//	rect2D = rect;
//	Initialize();
//}
void CollisionRect::Initialize()
{
	rect2D = SimpleMath::Rectangle(0, 0, 1, 1);
	PhysicsServer::Instance->RegisterRect(this);
}
