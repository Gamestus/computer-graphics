#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float nw, float u, float v)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz, nw) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT4 normal;
};