#include "stdafx.h"
#include "InvincibleItem.h"
#include "../../Player/Player.h"
#include "../../Scene/GameScene.h"

void InvincibleItem::Update()
{
	MapChip::Update();
	D3DXQUATERNION multi;
	D3DXQuaternionRotationAxis(&multi, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f * cPI / 180.0f);
	D3DXQuaternionMultiply(&m_rotation, &m_rotation, &multi);
	//プレイヤーとの距離が一定範囲内になったら消える
	D3DXVECTOR3 distance = GetGameScene().GetPlayer()->GetPosition() - m_position;
	if (D3DXVec3Length(&distance) < 8.0f)
	{
		MapChipDelete();
		m_pPlayer->Invincible();
	}
	m_skinModel.Update(m_position, m_rotation, m_scale);
}