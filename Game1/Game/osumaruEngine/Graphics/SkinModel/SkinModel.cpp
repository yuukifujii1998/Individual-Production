#include "engineStdafx.h"
#include "SkinModel.h"
#include "SkinModelData.h"
#include "../../Engine.h"
#include "../EffectManager.h"
#include "../Light.h"
#include "../Texture.h"
#include "../../Camera.h"

extern UINT                 g_NumBoneMatricesMax;
extern D3DXMATRIXA16*       g_pBoneMatrices;

void DrawMeshContainer(
	LPDIRECT3DDEVICE9 pd3dDevice,
	LPD3DXMESHCONTAINER pMeshContainerBase,
	LPD3DXFRAME pFrameBase,
	LPD3DXEFFECT pEffect,
	D3DXMATRIX* worldMatrix,
	D3DXMATRIX* rotationMatrix,
	D3DXMATRIX* viewMatrix,
	D3DXMATRIX* projMatrix,
	Light* light,
	bool isHasNormal,
	Texture* normalMap,
	bool isHasSpecular,
	Texture* specularMap,
	D3DXVECTOR3* cameraPos)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	UINT iAttrib;
	LPD3DXBONECOMBINATION pBoneComb;

	D3DXMATRIX matTemp;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);
	D3DXMATRIX viewProj;
	D3DXMatrixMultiply(&viewProj, viewMatrix, projMatrix);

	//テクニックを設定
	{
		if (pMeshContainer->pSkinInfo != NULL)
		{
			pEffect->SetTechnique("SkinModel");
		}
		else
		{
			pEffect->SetTechnique("NoSkinModel");
		}
	}
	//共通の定数レジスタを設定
	{
		//ビュープロジェクション
		pEffect->SetMatrix("g_mViewProj", &viewProj);
		//ライト
		pEffect->SetValue("g_light", light, sizeof(Light));
		if (isHasNormal && normalMap != nullptr)
		{
			pEffect->SetTexture("g_normalTexture", normalMap->GetBody());
		}
		if (isHasSpecular && specularMap != nullptr && cameraPos != nullptr)
		{
			pEffect->SetTexture("g_specularTexture", specularMap->GetBody());
			pEffect->SetFloatArray("g_cameraPos", *cameraPos, 3);
		}

		pEffect->SetBool("g_isHasNormalMap", isHasNormal);
		pEffect->SetBool("g_isHasSpecularMap", isHasSpecular);
	}
	if (pMeshContainer->pSkinInfo != NULL)
	{
		//スキン情報有り。
		//reinterpret_castはポインタからポインタへのキャスト(かなり危険。安全かはわからない
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			// first calculate all the world matrices
			for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
			{
				DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
				if (iMatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply(
						&g_pBoneMatrices[iPaletteEntry],
						&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
						pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
				}
			}

			pEffect->SetMatrixArray("g_mWorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries);
			pEffect->SetInt("g_numBone", pMeshContainer->NumInfi);
			//ディフューズテクスチャ
			pEffect->SetTexture("g_diffuseTexture", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
			//ボーン数。
			pEffect->SetInt("CurNumBones", pMeshContainer->NumInfi - 1);
			D3DXMATRIX viewRotInv;
			D3DXMatrixInverse(&viewRotInv, NULL, viewMatrix);
			viewRotInv.m[3][0] = 0.0f;
			viewRotInv.m[3][1] = 0.0f;
			viewRotInv.m[3][2] = 0.0f;
			viewRotInv.m[3][3] = 1.0f;
			D3DXMatrixTranspose(&viewRotInv, &viewRotInv);
			pEffect->SetMatrix("g_viewMatrixRotInv", &viewRotInv);
			pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
			pEffect->BeginPass(0);
			pEffect->CommitChanges();
			// draw the subset with the current world matrix palette and material state
			pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			pEffect->EndPass();
			pEffect->End();
		}
	}
	else
	{
		D3DXMATRIX mWorld;
		if (pFrame != NULL)
		{
			mWorld = pFrame->CombinedTransformationMatrix;
		}
		else
		{
			mWorld = *worldMatrix;
		}
		D3DXMATRIX mInvWorld;
		D3DXMatrixInverse(&mInvWorld, NULL, &mWorld);
		pEffect->SetBool("g_isHasNormalMap", false);
		pEffect->SetMatrix("g_worldMatrix", &mWorld);
		pEffect->SetMatrix("g_invWorldMatrix", &mInvWorld);
		pEffect->SetMatrix("g_rotationMatrix", rotationMatrix);
		pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
		pEffect->BeginPass(0);
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			pEffect->SetTexture("g_diffuseTexture", pMeshContainer->ppTextures[i]);
			pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		pEffect->EndPass();
		pEffect->End();
	}
}

void DrawFrame(
	LPDIRECT3DDEVICE9 pd3dDevice,
	LPD3DXFRAME pFrame,
	LPD3DXEFFECT pEffect,
	D3DXMATRIX* worldMatrix,
	D3DXMATRIX* rotationMatrix,
	D3DXMATRIX* viewMatrix,
	D3DXMATRIX* projMatrix,
	Light* light,
	bool isHasNormal,
	Texture* normalMap,
	bool isHasSpecular,
	Texture* specularMap,
	D3DXVECTOR3* cameraPos)
{
	LPD3DXMESHCONTAINER pMeshContainer;
	pMeshContainer = pFrame->pMeshContainer;
	while(pMeshContainer != NULL)
	{
		DrawMeshContainer(
			pd3dDevice,
			pMeshContainer,
			pFrame,
			pEffect,
			worldMatrix,
			rotationMatrix,
			viewMatrix,
			projMatrix,
			light,
			isHasNormal,
			normalMap,
			isHasSpecular,
			specularMap,
			cameraPos);

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(
			pd3dDevice,
			pFrame->pFrameSibling,
			pEffect,
			worldMatrix,
			rotationMatrix,
			viewMatrix,
			projMatrix,
			light,
			isHasNormal,
			normalMap,
			isHasSpecular,
			specularMap,
			cameraPos
			);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(
			pd3dDevice,
			pFrame->pFrameFirstChild,
			pEffect,
			worldMatrix,
			rotationMatrix,
			viewMatrix,
			projMatrix,
			light,
			isHasNormal,
			normalMap,
			isHasSpecular,
			specularMap,
			cameraPos);
	}
}

SkinModel::SkinModel()
{
	m_skinModelData = nullptr;
	m_light = nullptr;
	m_pEffect = nullptr;
	m_isHasNormalMap = false;
	m_isHasSpecularMap = false;
	m_pNormalMap = nullptr;
	m_pSpecularMap = nullptr;
	m_pCamera = nullptr;
}

SkinModel::~SkinModel()
{
}

void SkinModel::Init(SkinModelData* modelData)
{
	m_pEffect = GetEngine().GetEffectManager()->LoadEffect("Assets/Shader/Model.fx");
	m_skinModelData = modelData;
}

void SkinModel::UpdateWorldMatrix(D3DXVECTOR3 trans, D3DXQUATERNION rot, D3DXVECTOR3 scale)
{
	D3DXMATRIX mTrans, mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&mTrans, trans.x, trans.y, trans.z);
	D3DXMatrixRotationQuaternion(&m_rotationMatrix, &rot);
	m_worldMatrix = mScale * m_rotationMatrix * mTrans;

	if (m_skinModelData)
	{
		m_skinModelData->UpdateBoneMatrix(m_worldMatrix);		//ボーン行列を更新
	}
}

void SkinModel::Draw(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix)
{
	D3DXVECTOR3* cameraPos = nullptr;
	if (m_pCamera != nullptr)
	{
		cameraPos = &m_pCamera->GetPosition();
	}
	if (m_skinModelData)
	{
		DrawFrame(
			GetEngine().GetDevice(),
			m_skinModelData->GetFrameRoot(),
			m_pEffect,
			&m_worldMatrix,
			&m_rotationMatrix,
			viewMatrix,
			projMatrix,
			m_light,
			m_isHasNormalMap,
			m_pNormalMap,
			m_isHasSpecularMap,
			m_pSpecularMap,
			cameraPos);
	}
}

LPD3DXMESH SkinModel::GetOrgMeshFirst()
{
	if (m_skinModelData != nullptr)
	{
		return m_skinModelData->GetOrgMeshFirst();
	}
	return nullptr;
}