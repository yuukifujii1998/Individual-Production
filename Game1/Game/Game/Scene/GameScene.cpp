#include "stdafx.h"
#include "GameScene.h"
#include "../GameCamera/GameCamera.h"
#include "../Player/Player.h"
#include "../Map/Sky.h"
#include "../GameCamera/GameCamera.h"
#include "../Map/Map.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "Fade.h"

GameScene *g_gameScene;


GameScene::GameScene()
{
	m_isGameClear = false;
	m_isGameOver = false;
}

GameScene::~GameScene()
{

}

void GameScene::Start()
{
	m_sky = New<Sky>(0);
	m_map = New<Map>(0);
	m_map->Init();
	m_camera = New<GameCamera>(cameraPriority);
	m_camera->Init();
	g_pFade->FadeIn();
}

void GameScene::Update()
{
	if (!g_pFade->IsExcute())
	{
		if (g_pFade->GetCurrentState() == enFadeOut)
		{
			if (m_isGameClear)
			{
				New<GameClearScene>(0);
			}
			else if (m_isGameOver)
			{
				New<GameOverScene>(0);
			}
			Delete(this);
			g_gameScene = nullptr;
		}
	}
	else
	{
		return;
	}
}

Player* GameScene::GetPlayer()
{
	return m_map->GetPlayer();
}

Camera& GameScene::GetCamera()
{
	return m_camera->GetCamera();
}

void GameScene::BeforeDead()
{
	Delete(m_sky);
	Delete(m_map);
	Delete(m_camera);
}

void GameScene::GameClear()
{
	if (m_isGameClear)
	{
		return;
	}
	m_isGameClear = true;
	g_pFade->FadeOut();
}

void GameScene::GameOver()
{
	if (m_isGameOver)
	{
		return;
	}
	m_isGameOver = true;
	g_pFade->FadeOut();
}
