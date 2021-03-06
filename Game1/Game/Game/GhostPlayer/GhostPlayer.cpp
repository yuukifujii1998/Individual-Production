#include "stdafx.h"
#include "GhostPlayer.h"
#include "../Scene/GameScene.h"
#include "../GameCamera/GameCamera.h"
#include "../Player/Player.h"
#include "GhostDataListManager.h"

GhostPlayer::GhostPlayer() :
	m_modelData(),
	m_model(),
	m_animation(),
	m_ghostData(),
	m_currentAnimationNum(0),
	m_light(),
	m_isGoal(false),
	m_isActive(true)

{
}

GhostPlayer::~GhostPlayer()
{

}

bool GhostPlayer::Start()
{
	//ゴーストデータが無い場合デリート
	if (GetGhostDataListManager().GetGhostData().empty())
	{
		GetGameScene().GhostDataFinish();
		return true;
	}
	//プレイヤーをもとにモデルデータを作成
	const Player* player = GetGameScene().GetPlayer();
	m_modelData.CloneModelData(player->GetSkinModelData(), &m_animation);
	m_model.Init(&m_modelData);
	m_light = player->GetLight();
	m_light.SetAmbiemtLight({ 0.0f, 0.0f, 0.0f, 1.0f });
	m_model.SetLight(&m_light);
	m_model.SetShaderTechnique(enShaderTechniqueGhost);
	//ゴーストデータの最初のイテレーターを取得
	m_ghostData = GetGhostDataListManager().GetGhostData().begin();
	m_animation.PlayAnimation(m_ghostData->currentAnimationNum);
	m_currentAnimationNum = m_ghostData->currentAnimationNum;
	m_animation.SetAnimationLoopFlg(enAnimSetDelight, true);
	m_model.Update(m_ghostData->position, m_ghostData->rotation, { 1.0f, 1.0f, 1.0f });
	return true;
}


void GhostPlayer::Update()
{
	if (!m_isActive)
	{
		return;
	}
	//ゴールしてなければゴーストデータを流す
	if (!m_isGoal)
	{
		if (m_currentAnimationNum != m_ghostData->currentAnimationNum)
		{
			m_currentAnimationNum = m_ghostData->currentAnimationNum;
			m_animation.PlayAnimation(m_currentAnimationNum);
		}
		m_animation.Update(m_ghostData->animationUpdateTime);
		m_ghostData++;
		//ゴーストデータを流し終わったらゴールにする。
		if (GetGhostDataListManager().GetGhostData().end() == m_ghostData)
		{
			m_isGoal = true;
			m_ghostData--;
			m_ghostData--;
		}
	}
	else
	{
		//ゴールしたらジャンプして喜ぶモーションを再生
		if (m_currentAnimationNum != enAnimSetDelight)
		{
			m_currentAnimationNum = enAnimSetDelight;
			m_animation.PlayAnimation(enAnimSetDelight);
		}
		m_animation.Update(GetGameTime().GetDeltaFrameTime());
	}
	m_model.Update(m_ghostData->position, m_ghostData->rotation, { 1.0f, 1.0f, 1.0f });
}

void GhostPlayer::Draw()
{
	if (!m_isActive)
	{
		return;
	}
	const Camera& camera = GetGameScene().GetCamera();
	m_model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}