#include "engineStdafx.h"
#include "Sprite.h"
#include "../Engine.h"
#include "VertexCommon.h"
#include "EffectManager.h"

Sprite::Sprite() :
	m_alpha(1.0f),
	m_pTexture(nullptr),
	m_position(0.0f, 0.0f),
	m_centerPosition(0.0f, 0.0f),
	m_size(1.0f, 1.0f),
	m_pEffect(nullptr),
	m_primitive()
{
}

Sprite::~Sprite()
{
	Release();
}

void Sprite::Init(const Texture* texture)
{
	Release();
	//テクスチャを読み込み
	m_pTexture = texture;

	//スプライトの座標を初期化
	float centerPosx = m_pTexture->GetWidth() / 2.0f;
	float centerPosy = m_pTexture->GetHeight() / 2.0f;
	m_centerPosition = { centerPosx, centerPosy};
	SetPosition({ 0.0f, 0.0f});

	//エフェクトをロード
	m_pEffect = GetEffectManager().LoadEffect("Assets/shader/sprite.fx");

	//頂点バッファを作成
	VERTEX_PT elements[4] =
	{
		{ -1.0f,	1.0f,	0.0f,	1.0f,	0.0f ,	0.0f },
		{ 1.0f,		1.0f,	0.0f,	1.0f,	1.0f ,	0.0f },
		{ 1.0f,		-1.0f,	0.0f,	1.0f,	1.0f ,	1.0f },
		{ -1.0f,	-1.0f,	0.0f,	1.0f,	0.0f ,	1.0f },
	};
	//インデックスバッファーを作成
	WORD indexElements[6] = { 0, 2, 3, 0, 1, 2 };
	//プリミティブを作成
	m_primitive.Create(vertex_PT, elements, 4, sizeof(VERTEX_PT), indexElements, 6, Primitive::enIndex16, Primitive::enTypeTriangleList);
	m_size.x = m_pTexture->GetWidth();
	m_size.y = m_pTexture->GetHeight();
}

void Sprite::Draw()
{
	//座標のスケールを変換
	D3DXVECTOR3 position;
	position.x = m_position.x / (FRAME_BUFFER_WIDTH / 2.0f);
	position.y = m_position.y / (FRAME_BUFFER_HEIGHT / 2.0f);
	position.z = 0.0f;
	//拡大のスケールを変換
	D3DXVECTOR3 size;
	size.x = m_size.x / FRAME_BUFFER_WIDTH;
	size.y = m_size.y / FRAME_BUFFER_HEIGHT;
	size.z = 0.0f;

	//移動行列を作成
	D3DXMATRIX transform;
	D3DXMatrixTranslation(&transform, position.x, position.y, position.z);
	//拡大行列を作成
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, size.x, size.y, size.z);
	//ワールド行列を作成
	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scale);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &transform);
	LPDIRECT3DDEVICE9& pD3DDevice = GetEngine().GetDevice();
	DWORD srcBackup;
	DWORD destBackup;
	DWORD alphaBlendBackup;
	pD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphaBlendBackup);
	pD3DDevice->GetRenderState(D3DRS_SRCBLEND, &srcBackup);
	pD3DDevice->GetRenderState(D3DRS_DESTBLEND, &destBackup);

	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//描画
	m_pEffect->SetTechnique("Sprite");
	m_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_pEffect->BeginPass(0);
	m_pEffect->SetTexture("g_tex", m_pTexture->GetBody());
	m_pEffect->SetValue("g_world", worldMatrix, sizeof(worldMatrix));
	m_pEffect->SetFloat("g_alpha", m_alpha);
	m_pEffect->CommitChanges();
	pD3DDevice->SetVertexDeclaration(m_primitive.GetVertexDecaration());
	pD3DDevice->SetStreamSource(0, m_primitive.GetVertexBuffer(), 0, m_primitive.GetVertexStride());
	pD3DDevice->SetIndices(m_primitive.GetIndexBuffer());
	pD3DDevice->DrawIndexedPrimitive(m_primitive.GetPrimitiveType(), 0, 0, m_primitive.GetVertexNum(), 0, m_primitive.GetPolygonNum());

	m_pEffect->EndPass();
	m_pEffect->End();

	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlendBackup);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, srcBackup); 
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, destBackup);

}

void Sprite::Release()
{
	m_primitive.Release();
}