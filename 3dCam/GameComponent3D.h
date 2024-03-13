#pragma once
#include "GameComponent.h"

class GameComponent3D :
    public GameComponent
{
public:
	Vector3 GetGlobalPosition();
	Vector3 GetLocalPosition();
	Vector3 GetRotation();

	void SetLocalPosition(Vector3 pos);

	virtual void Update(float delta);
	virtual void Initialize();
	virtual void Draw();
	dx::XMMATRIX GetLocalTransform();
	dx::XMMATRIX GetGlobalTransform();
protected:
	Vector3 localPosition;
	Vector3 rotation;
	Vector3 scale = Vector3(1, 1, 1);
};