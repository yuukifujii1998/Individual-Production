#pragma once
#include "GameObject/GameObjectManager.h"
#include "Graphics\Sprite.h"
#include "Physics\Physics.h"
#include "Input\Pad.h"
#include "Graphics\ShadowMap.h"
#include "Sound\SoundEngine.h"
#include "Resource\TextureResource.h"
#include "Graphics\PostEffect\PostEffect.h"
#include "Resource\SkinModelDataResource.h"
#include "Graphics\CascadedShadowMap.h"
//エンジンクラス

const int FRAME_BUFFER_WIDTH = 1280;
const int FRAME_BUFFER_HEIGHT = 720;

class EffectManager;
class GameObject;


class Engine
{
private:
	//コンストラクタ。シングルトンのためプライベートになっている
	Engine();

	//デストラクタ。シングルトンのためプライベートになっている
	~Engine();
public:
	//DirectXを初期化
	void InitD3D(HINSTANCE& hInst);

	//ゲームループ
	void GameLoop();

	//ダイレクト3Dを取得
	LPDIRECT3D9& GetDirect3D()
	{
		return m_pD3D;
	}
	//デバイスを取得。
	LPDIRECT3DDEVICE9& GetDevice()
	{
		return m_pD3DDevice;
	}
	//エフェクトマネージャーを取得
	EffectManager& GetEffectManager()
	{
		return *m_effectManager;
	}
	//テクスチャリソースを取得
	TextureResource& GetTextureResource()
	{
		return m_textureResource;
	}
	//物理ワールドを取得。
	PhysicsWorld& GetPhysicsWorld()
	{
		return *m_physicsWorld;
	}

	//シャドウマップを取得
	CascadedShadowMap& GetShadowMap()
	{
		return m_shadowMap;
	}

	//ポストエフェクトを取得
	PostEffect& GetPostEffect()
	{
		return m_postEffect;
	}

	//スキンモデルデータリソースを取得
	SkinModelDataResource& GetModelDataResource()
	{
		return m_skinModelDataResource;
	}
	

	//メインのレンダリングターゲットを取得
	RenderTarget& GetMainRenderTarget()
	{
		return m_renderTarget[m_currentRenderTargetNum];
	}

	//メインレンダリングターゲットを切り替え
	void SwitchingRenderTarget()
	{
		m_currentRenderTargetNum ^= 1;
	}

	//自分のインスタンスを取得
	static Engine& GetEngine()
	{
		static Engine engine;
		return engine;
	}
	//リリース
	void Release()
	{
		if (m_pD3DDevice != NULL)
		{
			m_pD3DDevice->Release();
		}
		if (m_pD3D != NULL)
		{
			m_pD3D->Release();
		}
		PostQuitMessage(0);
	}

	//インスタンスの生成
	template<class T> 
	T* New(int priority)
	{
		return m_objectManager.New<T>(priority);
	}

	//インスタンスの削除
	void Delete(GameObject* deleteObject)
	{
		m_objectManager.Delete(deleteObject);
	}

	//インスタンスをオブジェクトマネージャーに登録
	void Add(GameObject* object, int priority)
	{
		m_objectManager.Add(object, priority);
	}

	//パッドの取得
	Pad& GetPad()
	{
		return m_pad;
	}

	//サウンドエンジンを取得
	SoundEngine& GetSoundEngine()
	{
		return m_soundEngine;
	}
private:
	LPDIRECT3D9								m_pD3D;						//DirectX9
	LPDIRECT3DDEVICE9						m_pD3DDevice;				//デバイス
	std::unique_ptr<EffectManager>			m_effectManager;			//エフェクトマネージャー
	WNDCLASSEX								m_wc;						//ウィンドウクラス
	GameObjectManager						m_objectManager;			//オブジェクトマネージャー
	std::unique_ptr<PhysicsWorld>			m_physicsWorld;				//物理ワールド
	Pad										m_pad;						//パッドの入力
	CascadedShadowMap								m_shadowMap;				//シャドウマップ
	SoundEngine								m_soundEngine;				//サウンドエンジン
	TextureResource							m_textureResource;			//テクスチャーリソース
	SkinModelDataResource					m_skinModelDataResource;	//モデルデータリソース
	RenderTarget							m_renderTarget[2];			//メインのレンダリングターゲット
	int										m_currentRenderTargetNum;	//今のメインレンダリングターゲットの番号
	PostEffect								m_postEffect;				//ポストエフェクト
};
//エンジンクラスのインスタンスを取得。
static Engine& GetEngine()
{
	return Engine::GetEngine();
}

//インスタンスの生成
template <class T>
static T* New(int priority)
{
	return GetEngine().New<T>(priority);
}
//インスタンスの削除
static void Delete(GameObject* deleteObject)
{
	GetEngine().Delete(deleteObject);
}

static void Add(GameObject* object, int priority)
{
	GetEngine().Add(object, priority);
}

//パッドの取得
static Pad& GetPad()
{
	return GetEngine().GetPad();
}

//シャドウマップの取得
static CascadedShadowMap& GetShadowMap()
{
	return GetEngine().GetShadowMap();
}

static SoundEngine& GetSoundEngine()
{
	return GetEngine().GetSoundEngine();
}

static TextureResource& GetTextureResource()
{
	return GetEngine().GetTextureResource();
}

static PhysicsWorld& GetPhysicsWorld()
{
	return GetEngine().GetPhysicsWorld();
}

static EffectManager& GetEffectManager()
{
	return GetEngine().GetEffectManager();
}

static RenderTarget& GetMainRenderTarget()
{
	return GetEngine().GetMainRenderTarget();
}

static const DepthOfField& GetDepthOfField()
{
	return GetEngine().GetPostEffect().GetDepthOfField();
}

static SkinModelDataResource& GetModelDataResource()
{
	return GetEngine().GetModelDataResource();
}

static PostEffect& GetPostEffect()
{
	return GetEngine().GetPostEffect();
}
