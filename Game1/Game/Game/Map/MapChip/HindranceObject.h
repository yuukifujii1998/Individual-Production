#pragma once
#include "MapChip.h"
//移動速度に加速度がついたMoveFloor

class HindranceObject : public MapChip
{
public:
	//コンストラクタ
	HindranceObject();

	//デストラクタ
	~HindranceObject();

	/*
	初期化する関数
	position	座標
	rotation	回転
	modelName	読み込むモデルの名前
	anim		アニメーション付きのモデルの場合アニメーションを入れる入れ物
	*/
	void Init(const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const char* modelName, Animation* anim = nullptr)override;

	//更新関数
	void Update()override;

	//描画関数
	void Draw()override;
private:
	D3DXVECTOR3		m_moveSpeed;		//移動速度
	D3DXVECTOR3		m_acceleration;		//加速度
	RigidBody		m_rigidBody;		//剛体
	BoxCollider		m_boxCollider;		//コライダー
	bool			m_isChild;			//子供がいるかどうか
	const int		m_timerLimit;		//タイマーの限界
	int				m_timer;			//一定時間たつと移動速度を反転させるためのタイマー
};