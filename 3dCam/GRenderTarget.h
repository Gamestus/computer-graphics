
#pragma once
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>


class Game;

class GRenderTarget
{
public:
	GRenderTarget(Game* render);

	bool Init();
	void Close();

	void SetRenderTarget();
	void ClearRenderTarget(float, float, float, float);

	// shader resource view
	ID3D11ShaderResourceView* GetShaderResourceView();


	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}

private:
	DirectX::XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;

	Game* pGame;

	ID3D11Texture2D* m_RTTexture;
	ID3D11Texture2D* m_DSTexture;
	ID3D11RenderTargetView* m_RTV;
	ID3D11ShaderResourceView* m_SRV;
	ID3D11DepthStencilView* m_DSV;
};