#pragma once
//ゲームカメラクラス

class GameCamera : public GameObject
{
public:
	//コンストラクタ
	GameCamera();
	
	//デストラクタ
	~GameCamera();

	//初期化関数
	void Init();

	bool Start()override;

	//更新関数
	void Update()override;

	//カメラの取得
	const Camera& GetCamera() const
	{
		return m_camera;
	}

private:
	D3DXQUATERNION		m_rotation;			//カメラの回転
	Camera				m_camera;			//カメラ
	float				m_angleX;			//カメラがX方向を軸に回転している角度	
	D3DXVECTOR3			m_position;			//プレイヤーのローカルでの初期座標
	const D3DXMATRIX*	m_playerBoneMat;	//カメラの注視点を決めるのに使うプレイヤーのボーンのワールド行列
};