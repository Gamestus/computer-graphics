#pragma once
#include "GameComponent.h"

class GameComponent3D :
    public GameComponent
{
public:
	Vector3 GetGlobalPosition();
	Vector3 GetLocalPosition();
	dx::XMMATRIX GetPositionMatrix();
	Vector3 GetRotation();
	dx::XMMATRIX GetRotationMatrix();

	void SetLocalPosition(Vector3 pos);
	void SetLocalRotation(Vector3 rot);

	virtual void Reparent(GameComponent* NewParent);
	virtual void Reparent(GameComponent3D* NewParent);
	virtual void Update(float delta);
	virtual void Initialize();
	virtual void Draw(bool isDepth = false);
	dx::XMMATRIX GetLocalTransform();
	dx::XMMATRIX GetGlobalTransform();
	dx::XMMATRIX GetParentTransform();
protected:
	Vector3 localPosition;
	Vector3 rotation;
	Vector3 scale = Vector3(1, 1, 1);
};