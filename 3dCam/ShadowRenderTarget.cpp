#include "ShadowRenderTarget.h"
#include "Game.h"


const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

ShadowRenderTarget::ShadowRenderTarget(Game* game)
{
	m_RTTexture = nullptr;
	m_RTV = nullptr;
	m_SRV = nullptr;
	m_DSTexture = nullptr;
	m_DSV = nullptr;
	pGame = game;
}

bool ShadowRenderTarget::Init(float screenNear, float screenDepth)
{

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = SHADOWMAP_WIDTH;
	textureDesc.Height = SHADOWMAP_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	if (FAILED(pGame->WrlDevice->CreateTexture2D(&textureDesc, NULL, &m_RTTexture)))
		return false;


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(pGame->WrlDevice->CreateRenderTargetView(m_RTTexture, &renderTargetViewDesc, &m_RTV)))
		return false;


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	if (FAILED(pGame->WrlDevice->CreateShaderResourceView(m_RTTexture, &shaderResourceViewDesc, &m_SRV)))
		return false;

	//Depth Stencil View


	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = SHADOWMAP_WIDTH;
	depthBufferDesc.Height = SHADOWMAP_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	if (FAILED(pGame->WrlDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_DSTexture)))
		return false;

	// depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(pGame->WrlDevice->CreateDepthStencilView(m_DSTexture, &depthStencilViewDesc, &m_DSV)))
		return false;

	m_viewport.Width = (float)SHADOWMAP_WIDTH;
	m_viewport.Height = (float)SHADOWMAP_HEIGHT;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(((float)DirectX::XM_PI / 4.0f), ((float)SHADOWMAP_WIDTH / (float)SHADOWMAP_HEIGHT), screenNear, screenDepth);

	return true;
}

void ShadowRenderTarget::Close()
{
	/*_RELEASE(m_DSV);
	_RELEASE(m_DSTexture);
	_RELEASE(m_SRV);
	_RELEASE(m_RTV);
	_RELEASE(m_RTTexture);*/
}

void ShadowRenderTarget::SetRenderTarget()
{
	pGame->DeviceContext->OMSetRenderTargets(1, &m_RTV, m_DSV);
	pGame->DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pGame->DeviceContext->RSSetViewports(1, &m_viewport);
}

void ShadowRenderTarget::ClearRenderTarget(float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };
	pGame->DeviceContext->ClearRenderTargetView(m_RTV, color);
	pGame->DeviceContext->ClearDepthStencilView(m_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* ShadowRenderTarget::GetShaderResourceView()
{
	return m_SRV;
}

DirectX::XMMATRIX ShadowRenderTarget::GetProjectionMatrix()
{
	return m_projectionMatrix;
}