#include "GameComponent3D.h"


Vector3 GameComponent3D::GetGlobalPosition()
{
	return globalPosition;
}

Vector3 GameComponent3D::GetLocalPosition()
{
	return localPosition;
}

void GameComponent3D::SetGlobalPosition(Vector3 pos) {
	globalPosition = pos;
	UpdatePosition(globalPosition); //TODO
}

void GameComponent3D::SetLocalPosition(Vector3 pos) {
	localPosition = pos;
	UpdatePosition(globalPosition); //TODO
}

void GameComponent3D::UpdatePosition(Vector3 parentGlobalPosition)
{
	globalPosition = parentGlobalPosition + localPosition;
	for (auto& child : children) {
		GameComponent3D* component3D = dynamic_cast<GameComponent3D*>(child.get());
		if (component3D) {
			component3D->UpdatePosition(globalPosition);
		}
	}
}
