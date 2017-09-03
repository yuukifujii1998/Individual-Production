#pragma once

class EffectManager
{
public:

	EffectManager();

	~EffectManager();

	/*
	�G�t�F�N�g�̃��[�h
	�ǂݍ��ݍς݂̃G�t�F�N�g�̓��[�h�����Ɋ����̃G�t�F�N�g���Ԃ��Ă���
	*/
	LPD3DXEFFECT LoadEffect(const char* filePath);

	void Release();
private:
	std::map<int, LPD3DXEFFECT> effectDictionary;
};