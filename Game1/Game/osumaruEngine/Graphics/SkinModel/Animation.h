#pragma once
//アニメーションを管理するクラス

class Animation
{
public:
	//コンストラクタ
	Animation();

	//デストラクタ
	~Animation();

	/*
	初期化
	anim		アニメーションコントローラー
	*/
	void Init(ID3DXAnimationController* anim);

	/*
	アニメーションの終了タイムを設定する。
	animationSetIndex			アニメーションインデックス。
	endTime						アニメーションの終了タイム。-1.0を設定するとアニメーションファイルに設定されている終了タイムになる
	*/
	void SetAnimationEndTime(int animationSetIndex, double endTime)
	{
		m_animationEndTime[animationSetIndex] = endTime;
	}
	/*
	アニメーションの再生
	animationIndex			再生したいアニメーションのインデックス
	*/
	void PlayAnimation(int animationSetIndex);

	/*
	アニメーションの再生。アニメーションの補間が行われます
	animationSetIndex				再生したいアニメーションのインデックス
	interpolateTime					補間時間。
	*/
	void PlayAnimation(int animationSetIndex, float interpolateTime);
#if 0
	/*
	アニメーションのブレンディング再生
	animationIndex			再生したいアニメーションのインデックス。
	*/
	void BlendAnimation(int animationSetIndex);
#endif
	/*
	アニメーションセットの取得
	*/
	int GetNumAnimationSet() const
	{
		return m_numAnimSet;
	}
	/*
	アニメーションの更新
	deltaTime		更新時間、単位は秒
	*/
	void Update(float deltaTime);

private:

	ID3DXAnimationController*				m_pAnimController;			//アニメーションコントローラー
	int										m_numAnimSet;					//アニメーションセットの数
	std::unique_ptr<ID3DXAnimationSet*[]>	m_animationSets;				//アニメーションの配列。
	std::unique_ptr<float[]>				m_blendRateTable;				//ブレンディングレートのテーブル。
	std::unique_ptr<double[]>				m_animationEndTime;			//アニメーションの終了タイム。デフォルトは-1.0が入っていて、-1.0が入っている場合はID3DXAnimationSetのアニメーション終了タイムが優先される。
																		//DirectX9のアニメーションセットに1秒以下のアニメーションを入れる方法が見つからない。一秒以下のアニメーションはこいつを適時設定。
	double									m_localAnimationTime;			//ローカルアニメーションタイム
	int										m_currentAnimationSetNo;		//現在再生中のアニメーショントラックの番号
	int										m_currentTrackNo;				//現在のトラックの番号
	int										m_numMaxTracks;				//アニメーショントラックの最大数
	bool									m_isBlending;					//アニメーションブレンディング中？
	bool									m_isInterpolate;				//補間中？
	float									m_interpolateEndTime;			//補間終了時間
	float									m_interpolateTime;			//補間時間
};	