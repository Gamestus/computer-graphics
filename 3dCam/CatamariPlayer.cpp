#include "CatamariPlayer.h"
#include "Keys.h"

CatamariPlayer::CatamariPlayer()
{
	Initialize();
}

void CatamariPlayer::Initialize()
{
    game = Game::Instance;
	centralMesh = AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models\\UV_sphere.obj", L"textures\\obama_c.jpg"));
	Cam = AddChild<Camera>(std::make_unique<Camera>());
    game->PhysServer->player = &collision;
    game->PhysServer->CatamariPlayer = this;
}


void ExtractPitchYawRollFromXMMatrix(float* flt_p_PitchOut, float* flt_p_YawOut, float* flt_p_RollOut, const DirectX::XMMATRIX* XMMatrix_p_Rotation)
{
    DirectX::XMFLOAT4X4 XMFLOAT4X4_Values;
    DirectX::XMStoreFloat4x4(&XMFLOAT4X4_Values, DirectX::XMMatrixTranspose(*XMMatrix_p_Rotation));
    *flt_p_PitchOut = (float)asin(-XMFLOAT4X4_Values._23);
    *flt_p_YawOut = (float)atan2(XMFLOAT4X4_Values._13, XMFLOAT4X4_Values._33);
    *flt_p_RollOut = (float)atan2(XMFLOAT4X4_Values._21, XMFLOAT4X4_Values._22);
}

void CatamariPlayer::Update(float delta)
{
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


    moveInput = dx::XMVector3Normalize(moveInput);
    moveInput *= moveSpeed * delta;

    dx::XMVECTOR transformedMoveInput = dx::XMVector3Transform(
        dx::XMLoadFloat3(&moveInput),
        rotationMatrix
    );

    dx::XMFLOAT3 transformedMovePosition;
    dx::XMStoreFloat3(&transformedMovePosition, transformedMoveInput);

    //rotation
    dx::XMVECTOR upVector = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    dx::XMVECTOR rotationAxis = dx::XMVector3Cross(upVector, transformedMoveInput);
    rotationAxis = dx::XMVector3Normalize(rotationAxis);

    assert(rotationAxis.m128_f32[1] == 0.0);

    auto localPosition = GetLocalPosition();
    localPosition += transformedMovePosition;
    SetLocalPosition(localPosition);

    if (!dx::XMVector3Equal(rotationAxis, dx::XMVectorZero()))
    {
        float rotationAngle = dx::XMVectorGetX(dx::XMVector3Length(transformedMoveInput));
        dx::XMMATRIX rotationMatrix = dx::XMMatrixRotationAxis(rotationAxis, rotationAngle);
        dx::XMMATRIX transform = GetGlobalTransform();
        dx::XMMATRIX transformedMatrix =  transform * rotationMatrix;
        float yaw, pitch, roll;
        ExtractPitchYawRollFromXMMatrix(&yaw, &pitch, &roll, &transformedMatrix);
        rotation.x = yaw;
        rotation.y = pitch;
        rotation.z = roll;
    }
}

void CatamariPlayer::Eat(AssimpMesh* mesh)
{
    mesh->Reparent((GameComponent3D*)this);
    mesh->IsCollision = false;
    size += 0.03;
    collision.Radius = size;
    Cam->distance = 5.0f + size;
    if (moveSpeed < 20.0f) moveSpeed = 1.0f + size * size;
}

