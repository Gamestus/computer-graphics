#include "CatamariPlayer.h"

CatamariPlayer::CatamariPlayer()
{
	Initialize();
}

void CatamariPlayer::Initialize()
{
	centralMesh = AddChild<AssimpMesh>(std::make_unique<AssimpMesh>("models\\UV_sphere.obj", L"textures\\saul.jpg"));
	Cam = AddChild<Camera>(std::make_unique<Camera>());
}
