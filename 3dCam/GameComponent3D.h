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
	virtual void Update(float delta);
	virtual void Initialize();
	virtual void Draw();
protected:
	Vector3 globalPosition;
	Vector3 localPosition;
	Vector3 rotation;
private:
	void UpdatePosition(Vector3 parentGlobalPosition);
};