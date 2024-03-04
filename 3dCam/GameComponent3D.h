#pragma once
#include "GameComponent.h"

namespace dx = DirectX;

class GameComponent3D :
    public GameComponent
{
public:
	Vector3 GetGlobalPosition();
	Vector3 GetLocalPosition();

	void SetGlobalPosition(Vector3 pos);
	void SetLocalPosition(Vector3 pos);
protected:
	Vector3 globalPosition;
	Vector3 localPosition;
private:
	void UpdatePosition(Vector3 parentGlobalPosition);
};