#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float w, float nx, float ny, float nz, float nw, float u, float v)
		: pos(x, y, z, w), normal(nx, ny, nz, nw), texCoord(u, v) {}

	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT2 texCoord;
};