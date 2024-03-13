#include "GameComponent3D.h"


Vector3 GameComponent3D::GetGlobalPosition()
{
	return GetGlobalTransform().r[3];
}

Vector3 GameComponent3D::GetLocalPosition()
{
	return localPosition;
}

void GameComponent3D::SetLocalPosition(Vector3 pos) {
	localPosition = pos;
}

void GameComponent3D::Update(float delta)
{
}

void GameComponent3D::Draw()
{
}

dx::XMMATRIX GameComponent3D::GetLocalTransform()
{
	dx::XMMATRIX translationMatrix = dx::XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);
	dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	dx::XMMATRIX scaleMatrix = dx::XMMatrixScaling(scale.x, scale.y, scale.z);

	return scaleMatrix * rotationMatrix * translationMatrix;
}

dx::XMMATRIX GameComponent3D::GetGlobalTransform()
{
	dx::XMMATRIX parentTransform = dx::XMMatrixIdentity();

	if (auto parentComponent = dynamic_cast<GameComponent3D*>(parent.get()))
	{
		parentTransform = parentComponent->GetGlobalTransform();
	}

	return parentTransform * GetLocalTransform();
}

void GameComponent3D::Initialize()
{
}