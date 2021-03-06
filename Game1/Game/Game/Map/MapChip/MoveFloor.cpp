#include "stdafx.h"
#include "MoveFloor.h"
#include "../../Scene/GameScene.h"
#include "../../Player/Player.h"

MoveFloor::MoveFloor() :
	m_timer(0.0f),
	m_moveSpeed(0.0f, 0.0f, 0.0f),
	m_rigidBody(),
	m_boxCollider(),
	m_isChild(false)
{

}

MoveFloor::~MoveFloor()
{

}

void MoveFloor::Init(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const char *modelName, Animation* anim)
{
	MapChip::Init(position, rotation, modelName);

	//メッシュコライダーからaabbを作成	
	MeshCollider meshCollider;
	meshCollider.CreateFromSkinModel(&m_skinModel, NULL);
	D3DXVECTOR3 boxSize = (meshCollider.GetAabbMax() - meshCollider.GetAabbMin()) / 2.0f;
	m_boxCollider.Create({ boxSize.x, boxSize.y, boxSize.z });

	RigidBodyInfo rInfo;
	rInfo.collider = &m_boxCollider;
	rInfo.mass = 0.0f;
	rInfo.pos = m_position;
	rInfo.rot = m_rotation;

	//剛体を作成
	m_rigidBody.Create(rInfo);
	m_rigidBody.SetUserIndex(enCollisionAttr_MoveFloor);
	m_rigidBody.SetPlayerCollisionGroundFlg(false);

	//ワールド行列をもとに移動方向を設定
	D3DXMATRIX worldMatrix = m_skinModel.GetWorldMatrix();
	m_moveSpeed.x = worldMatrix.m[2][0];
	m_moveSpeed.y = worldMatrix.m[2][1];
	m_moveSpeed.z = worldMatrix.m[2][2];
	D3DXVec3Normalize(&m_moveSpeed, &m_moveSpeed);
	m_moveSpeed *= 0.2f;
	m_skinModel.SetShaderTechnique(enShaderTechniqueDithering);
}

void MoveFloor::Update()
{
	MapChip::Update();
	if (!m_isActive)
	{
		true;
	}
	m_position += m_moveSpeed;

	//子供がいない状態でプレイヤーが当たったら親子関係をつける
	if (!m_isChild && (m_rigidBody.GetBody()->getPlayerCollisionGroundFlg() || m_rigidBody.GetBody()->getPlayerCollisionWallFlg()))
	{
		m_isChild = m_pPlayer->SetParent(this, true);
	}
	//プレイヤーが子供の時にプレイヤーが離れたたら親子関係を切る
	if (m_isChild && !m_rigidBody.GetBody()->getPlayerCollisionGroundFlg() && !m_rigidBody.GetBody()->getPlayerCollisionWallFlg())
	{
		
		m_isChild = m_pPlayer->SetParent(nullptr, true);
	}
	m_timer += 1.0f / 60.0f;
	if (5.0f < m_timer)
	{
		m_moveSpeed *= -1.0f;
		m_timer = 0.0f;
	}

	//剛体のワールド行列を更新
	m_rigidBody.SetPosition(m_position);
	m_rigidBody.SetRotation(m_rotation);
	m_rigidBody.SetPlayerCollisionGroundFlg(false);
	m_rigidBody.SetPlayerCollisionWallFlg(false);

	m_skinModel.Update(m_position, m_rotation, m_scale);
}

void MoveFloor::Draw()
{
	MapChip::Draw();
	GetPhysicsWorld().DebugDraw(m_rigidBody.GetBody()->getWorldTransform(), m_rigidBody.GetBody()->getCollisionShape());
}