#include "engineStdafx.h"
#include "Texture.h"
#include "../Engine.h"

Texture::Texture() :
	m_pTexture(nullptr),
	m_srcInfo{}
{
}

Texture::~Texture()
{
	Release();
}

void Texture::Load(const char *filePath)
{
	Release();
	D3DXCreateTextureFromFileEx(
		GetEngine().GetDevice(),
		filePath,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		&m_srcInfo,
		NULL,
		&m_pTexture
	);
}

void Texture::Release()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}