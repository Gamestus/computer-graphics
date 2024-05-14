#pragma once
#include <d3d11.h>
#include <wrl.h>


class Buffer
{
public:
	static ID3D11Buffer* CreateVertexBuffer(ID3D11Device* device, int size, bool dynamic, const void* Mem);
	static ID3D11Buffer* CreateIndexBuffer(ID3D11Device* device, int size, bool dynamic, const void* Mem);
	static ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, int size, bool dynamic);
};
