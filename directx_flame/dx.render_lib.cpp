#include "dx.render_lib.h"
#include <d3dx9math.h>

IDirect3DDevice9*	pD3Device;		//	Direct3Dのデバイス

D3DPRESENT_PARAMETERS d3dpp;		//	パラメーター




void Vertex_Spin(CUSTOMVERTEX* vertex, float spin_speed, CUSTOMVERTEX* temp)
{
	D3DXToRadian(spin_speed);
	for (int i = 0; i < 4; i++)
	{
		vertex[i] = temp[i];
		vertex[i].x = temp[i].x * cos(spin_speed) - temp[i].y * sin(spin_speed);
		vertex[i].y = temp[i].x * sin(spin_speed) + temp[i].y * cos(spin_speed);
	}
}

void Vertex_Scaling(CUSTOMVERTEX* vertex, float x, float y)
{
	for (int count = 0; count < 4; count++)
	{
		vertex[count].x = vertex[count].x * x;
		vertex[count].y = vertex[count].y * y;
	}
}

void RenderSet()
{
	//描画方法の設定
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD3Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

}


void BeginScene()
{
	//画面の消去
	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//描画の開始
	pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	pD3Device->BeginScene();

}

void EndScene()
{
	//描画の終了
	pD3Device->EndScene();

	//表示
	pD3Device->Present(NULL, NULL, NULL, NULL);
}



void Tex_Draw(LPDIRECT3DTEXTURE9 *pTexture, CUSTOMVERTEX* VertexName, int TexID)
{
	pD3Device->SetTexture(0, pTexture[TexID]);
	pD3Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		VertexName,
		sizeof(CUSTOMVERTEX));
}


HRESULT Tex_Load_EX(LPDIRECT3DTEXTURE9 *pTexture,const char* text, int TexID, int alpha, int red, int green, int blue)
{
	return D3DXCreateTextureFromFileEx(
		pD3Device,
		TEXT(text),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		D3DCOLOR_ARGB(alpha, red, green, blue),
		NULL, NULL,
		&pTexture[TexID]);
}

void Mesh_Load_FromX(LPTSTR xfilename, pTHING pThing, D3DXVECTOR3* pvecPosition)
{
	memcpy(&pThing->vecPosition,pvecPosition,sizeof(D3DXVECTOR3));

	LPD3DXBUFFER	pMatBuf = NULL;

	if (FAILED(D3DXLoadMeshFromX(
		xfilename,				// xファイルのファイルパス
		D3DXMESH_MANAGED,	// 頂点バッファ作成オプション
		pD3Device,				// Direct3DDeviceポインタ
		NULL,				// 隣接性データポリゴン情報 使わない
		&pMatBuf,			// マテリアル情報
		NULL,				// エフェクト 使わない
		&pThing->nMat,				// メッシュの数取得
		&pThing->pMesh			// メッシュ情報のポインタ
		)))
	{
		MessageBox(NULL, "xファイルの読み込みに失敗しました", xfilename, MB_OK);
	}

	if (!(pThing->pMesh->GetFVF() & D3DFVF_NORMAL)) {

		ID3DXMesh* pTempMesh = NULL;

		pThing->pMesh->CloneMeshFVF(pThing->pMesh->GetOptions(),
			pThing->pMesh->GetFVF() | D3DFVF_NORMAL, pD3Device, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		pThing->pMesh->Release();
		pThing->pMesh = pTempMesh;
	}

	// マテリアル情報を取り出す
	D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
	pThing->pMeshMat = new D3DMATERIAL9[pThing->nMat];		// メッシュ情報を確保
	pThing->pMeshTex = new LPDIRECT3DTEXTURE9[pThing->nMat];// テクスチャを確保
	for (int i = 0; i < pThing->nMat; i++)
	{
		pThing->pMeshMat[i] = d3Mat[i].MatD3D;			// マテリアル情報セット
		pThing->pMeshMat[i].Ambient = pThing->pMeshMat[i].Diffuse;// 環境光初期化
		pThing->pMeshTex[i] = NULL;	// テクスチャ初期化

		// 使用しているテクスチャがあれば読み込む
		if (d3Mat[i].pTextureFilename != NULL &&
			lstrlen(d3Mat[i].pTextureFilename) > 0)
		{
			// テクスチャ読み込み
			if(FAILED(D3DXCreateTextureFromFile(
				pD3Device,
				d3Mat[i].pTextureFilename,
				&pThing->pMeshTex[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	// マテリアル情報開放
	pMatBuf->Release();



}

void Set_Transform(THING* pThing,float fScale)
{
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMATRIXA16 matWorld, matPosition,matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPosition, pThing->vecPosition.x, pThing->vecPosition.y,
		pThing->vecPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

	D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
	pD3Device->SetTransform(D3DTS_WORLD, &matWorld);
}


void Set_View_Light(FLOAT Eye_x, FLOAT Eye_y, FLOAT Eye_z)
{
	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vecEyePt(Eye_x, Eye_y, Eye_z); //カメラ（視点）位置
	D3DXVECTOR3 vecLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vecEyePt, &vecLookatPt, &vecUpVec);
	pD3Device->SetTransform(D3DTS_VIEW, &matView);
	// プロジェクショントランスフォーム（射影変換）
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 100.0f);
	pD3Device->SetTransform(D3DTS_PROJECTION, &matProj);
	// ライトをあてる 白色で鏡面反射ありに設定
	D3DXVECTOR3 vecDirection(1, 1, 1);
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);
	light.Range = 200.0f;
	pD3Device->SetLight(0, &light);
	pD3Device->LightEnable(0, TRUE);
}

void Draw_Thing(THING* pThing)
{
	// レンダリング	 
	for (DWORD i = 0; i<pThing->nMat; i++)
	{
		pD3Device->SetMaterial(&pThing->pMeshMat[i]);
		pD3Device->SetTexture(0, pThing->pMeshTex[i]);
		pThing->pMesh->DrawSubset(i);
	}
}


void Transform_Draw_Thing(THING* pThing,float fScale)
{
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMATRIXA16 matWorld, matPosition, matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPosition, pThing->vecPosition.x, pThing->vecPosition.y,pThing->vecPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

	D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
	pD3Device->SetTransform(D3DTS_WORLD, &matWorld);

	// レンダリング	 
	for (DWORD i = 0; i<pThing->nMat; i++)
	{
		pD3Device->SetMaterial(&pThing->pMeshMat[i]);
		pD3Device->SetTexture(0, pThing->pMeshTex[i]);
		pThing->pMesh->DrawSubset(i);
	}

}

HRESULT Tex_Load(LPDIRECT3DTEXTURE9 *pTexture, const char* name, int TexID)
{
#if 1
	// ここでエラー処理しないのなら、呼び元にそのままエラーコード返しちゃえば？
	return D3DXCreateTextureFromFile(
		pD3Device,
		TEXT(name),
		&pTexture[TexID]);
#else
	HRESULT hr;
	if (FAILED(hr=D3DXCreateTextureFromFile(
		pD3Device,
		TEXT(name),
		&pTexture[TexID])))
	{
		return hr;
	}
	return S_OK;
#endif
}
