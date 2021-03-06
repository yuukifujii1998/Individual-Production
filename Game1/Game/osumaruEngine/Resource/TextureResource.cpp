#include "engineStdafx.h"
#include "TextureResource.h"
#include "../Graphics/Texture.h"

TextureResource::TextureResource()
{

}

TextureResource::~TextureResource()
{
	for (auto& map : m_textures)
	{
		map.second->Release();
	}
}

Texture* TextureResource::LoadTexture(const char* filePath)
{
	int hash = MakeHash(filePath);
	auto& map = m_textures.find(hash);
	if (map == m_textures.end())
	{
		Texture* texture = new Texture;
		texture->Load(filePath);
		m_textures.insert({ hash, texture });
		return texture;
	}
	else
	{
		return map->second;
	}
}