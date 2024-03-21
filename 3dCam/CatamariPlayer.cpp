#include "CatamariPlayer.h"
#include "Keys.h"

CatamariPlayer::CatamariPlayer()
{
	Initialize();
}

void CatamariPlayer::Initialize()
{
    game = Game::Instance;
	centralMesh = AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models\\UV_sphere.obj", L"textures\\saul.jpg"));
	Cam = AddChild<Camera>(std::make_unique<Camera>());
    game->PhysServer->player = &collision;
    game->PhysServer->CatamariPlayer = this;
}


void CatamariPlayer::Update(float delta)
{
    collision.Radius = 1.0;
    collision.Center = GetGlobalPosition();
    Vector3 moveInput;
    if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::W))) {
        moveInput.z = 1;
    }
    else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::S))) {
        moveInput.z = -1;
    }

    if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::D))) {
        moveInput.x = 1;
    }
    else if (Game::Instance->InDevice->IsKeyDown(static_cast<u_char>(Keys::A))) {
        moveInput.x = -1;
    }

    dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationY( -Cam->GetTheta() );

    moveInput *= moveSpeed * delta;

    dx::XMVECTOR transformedMoveInput = dx::XMVector3Transform(
        dx::XMLoadFloat3(&moveInput),
        rotationMatrix
    );

    dx::XMFLOAT3 transformedMovePosition;
    dx::XMStoreFloat3(&transformedMovePosition, transformedMoveInput);

    //rotation
    dx::XMVECTOR upVector = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    dx::XMVECTOR rotationAxis = dx::XMVector3Cross(transformedMoveInput, upVector);
    rotationAxis = dx::XMVector3Normalize(rotationAxis);

    if (!dx::XMVector3Equal(rotationAxis, dx::XMVectorZero()))
    {
        float rotationAngle =  -dx::XMVectorGetX(dx::XMVector3Length(transformedMoveInput));

        dx::XMVECTOR yawPitchRoll = dx::XMQuaternionRotationAxis(rotationAxis, rotationAngle);
        rotation += yawPitchRoll;
    }


    auto localPosition = GetLocalPosition();
    localPosition += transformedMovePosition;
    SetLocalPosition(localPosition);
}