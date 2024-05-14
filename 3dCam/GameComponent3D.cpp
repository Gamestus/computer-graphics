#include "GameComponent3D.h"


Vector3 GameComponent3D::GetGlobalPosition()
{
	return GetGlobalTransform().r[3];
}

dx::XMMATRIX GameComponent3D::GetGlobalPositionMatrix()
{
	Vector3 position = GetGlobalPosition();
	return dx::XMMatrixTranslation(position.x, position.y, position.z);;
}

Vector3 GameComponent3D::GetLocalPosition()
{
	return localPosition;
}

Vector3 GameComponent3D::GetRotation()
{
	return rotation;
}

dx::XMMATRIX GameComponent3D::GetRotationMatrix()
{
	return dx::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
}

void GameComponent3D::SetLocalPosition(Vector3 pos) {
	localPosition = pos;
}

void GameComponent3D::SetLocalRotation(Vector3 rot) {
	rotation = rot;
}

void GameComponent3D::Reparent(GameComponent* NewParent)
{
	GameComponent::Reparent(NewParent);
}

void GameComponent3D::Reparent(GameComponent3D* NewParent)
{
	dx::XMMATRIX oldGlobalTransform = GetGlobalTransform();


	GameComponent::Reparent((GameComponent*)NewParent);
	dx::XMMATRIX newParentInverseTransform = dx::XMMatrixInverse(nullptr, GetParentTransform());

	dx::XMMATRIX newLocalTransform = oldGlobalTransform * newParentInverseTransform;

	dx::XMVECTOR newScale;
	dx::XMVECTOR newRotation;
	dx::XMVECTOR newPosition;
	dx::XMMatrixDecompose(&newScale, &newRotation, &newPosition, newLocalTransform);



	SetLocalPosition(newPosition);
	SetLocalRotation(newRotation);
}

void GameComponent3D::Update(float delta)
{
}

void GameComponent3D::Draw(bool isDepth)
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
	return GetLocalTransform() * GetParentTransform();
}

dx::XMMATRIX GameComponent3D::GetParentTransform()
{
	dx::XMMATRIX parentTransform = dx::XMMatrixIdentity();

	if (auto parentComponent = dynamic_cast<GameComponent3D*>(parent))
	{
		parentTransform = parentComponent->GetGlobalTransform();
	}
	return parentTransform;
}

void GameComponent3D::Initialize()
{
}