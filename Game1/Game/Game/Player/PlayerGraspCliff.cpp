#include "stdafx.h"
#include "PlayerGraspCliff.h"
#include "Player.h"

PlayerGraspCliff::PlayerGraspCliff() :
	m_cliffRiseDetectionLow(),
	m_cliffRiseDetectionUp(),
	m_cliffRiseDetectionBack(),
	m_boxCollider(),
	m_player(nullptr),
	m_playerHeight(0.0f),
	m_isActive(false)

{
}

PlayerGraspCliff::~PlayerGraspCliff()
{

}

void PlayerGraspCliff::Init(Player* player, float height)
{
	m_player = player;
	m_playerHeight = height;

	D3DXVECTOR3 position  = m_player->GetPosition();
	D3DXMATRIX matrix = m_player->GetWorldMatrix();
	D3DXQUATERNION rotation;
	D3DXQuaternionRotationMatrix(&rotation, &matrix);
	position.y += m_playerHeight;

	m_boxCollider.Create({ 1.0f, 0.4f, 0.4f });

	m_cliffRiseDetectionBack.Init(&m_boxCollider, position, rotation);
	m_cliffRiseDetectionLow.Init(&m_boxCollider, position, rotation);
	m_cliffRiseDetectionUp.Init(&m_boxCollider, position, rotation);
	m_cliffRiseDetectionBack.SetJudgmentType(enJudgment_Wall);
	m_cliffRiseDetectionLow.SetJudgmentType(enJudgment_Wall);
	m_cliffRiseDetectionUp.SetJudgmentType(enJudgment_Wall);
}

void PlayerGraspCliff::Update()
{
	float m_upDifference = 1.0f;
	float m_backDifference = 1.0f;
	float m_frontDifference = 1.0f;

	//ワールド行列から回転行列を引っ張ってきて剛体の回転を設定
	D3DXMATRIX worldMatrix = m_player->GetWorldMatrix();
	D3DXQUATERNION rotation;
	D3DXQuaternionRotationMatrix(&rotation, &worldMatrix);
	m_cliffRiseDetectionLow.SetRotation(rotation);
	m_cliffRiseDetectionUp.SetRotation(rotation);
	m_cliffRiseDetectionBack.SetRotation(rotation);

	//回転行列を使って剛体の位置を調整
	D3DXVECTOR3 position = m_player->GetPosition();
	D3DXVECTOR3 playerFrontNormal;
	playerFrontNormal.x = worldMatrix.m[2][0];
	playerFrontNormal.y = 0.0f;
	playerFrontNormal.z = worldMatrix.m[2][2];
	D3DXVec3Normalize(&playerFrontNormal, &playerFrontNormal);
	position.y += m_playerHeight;
	position += playerFrontNormal * m_backDifference;
	m_cliffRiseDetectionBack.SetPosition(position);
	position += playerFrontNormal * m_frontDifference;
	m_cliffRiseDetectionLow.SetPosition(position);
	position.y += m_upDifference;
	m_cliffRiseDetectionUp.SetPosition(position);

	m_cliffRiseDetectionLow.Execute();
	m_cliffRiseDetectionUp.Execute();
	m_cliffRiseDetectionBack.Execute();

	//剛体が当たっていて崖を上っていない時
	if (m_cliffRiseDetectionLow.IsHit() &&
		!m_cliffRiseDetectionUp.IsHit() &&
		!m_cliffRiseDetectionBack.IsHit() &&
		m_cliffRiseDetectionLow.GetHitCollisionType() != enCollisionAttr_Rotation &&
		!m_isActive)
	{
		D3DXVECTOR3 collisionNormal = m_cliffRiseDetectionLow.GetCollisionNormal();
		D3DXVec3Normalize(&collisionNormal, &collisionNormal);
		if (D3DXVec3Dot(&playerFrontNormal, &collisionNormal) < 0.0f)
		{
			collisionNormal *= -1.0f;
		}
		m_player->CliffRiseStart(collisionNormal);
		m_isActive = true;
	}

	//崖を上っている時
	if (m_isActive)
	{
		m_isActive = !m_player->CriffRiseEnd();
	}
}

void PlayerGraspCliff::Draw()
{
	//剛体を描画
	const int detectionNum = 3;
	CollisionDetection* detection[detectionNum] =
	{
		&m_cliffRiseDetectionBack,
		&m_cliffRiseDetectionUp,
		&m_cliffRiseDetectionLow
	};
	for (int i = 0; i < detectionNum; i++)
	{
		D3DXVECTOR3 position = detection[i]->GetPosition();
		D3DXQUATERNION rotation = detection[i]->GetRotation();
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(position.x, position.y, position.z));
		trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

		GetPhysicsWorld().DebugDraw(trans, m_boxCollider.GetBody());
	}
}