#pragma once
//作りかけのクラス

class Font
{
public:
	void Init(char* chara);

	void Draw();
private:
	LPDIRECT3DTEXTURE9 m_pTexture;

};