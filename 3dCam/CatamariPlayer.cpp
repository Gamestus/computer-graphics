#include "CatamariPlayer.h"
#include "Keys.h"

CatamariPlayer::CatamariPlayer()
{
	Initialize();
}

void CatamariPlayer::Initialize()
{
	centralMesh = AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models\\UV_sphere.obj", L"textures\\saul.jpg"));
	Cam = AddChild<Camera>(std::make_unique<Camera>());
}


void CatamariPlayer::Update(float delta)
{

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

    // Obtain the rotation matrix of the camera
    dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationY( -Cam->GetTheta() );

    // Transform the moveInput vector based on the rotation matrix
    dx::XMVECTOR transformedMoveInput = dx::XMVector3Transform(
        dx::XMLoadFloat3(&moveInput),
        rotationMatrix
    );

    // Extract the transformed values from the vector
    dx::XMFLOAT3 transformedMovePosition;
    dx::XMStoreFloat3(&transformedMovePosition, transformedMoveInput);

    // Set the local position using the transformed move position
    SetLocalPosition(GetLocalPosition() + transformedMovePosition);
}