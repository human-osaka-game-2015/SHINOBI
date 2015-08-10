#include "dx.render_lib.h"
#include <d3dx9math.h>
IDirect3DDevice9*	pD3Device;		//	Direct3Dのデバイス

D3DPRESENT_PARAMETERS d3dpp;		//	パラメーター




void Vertex_Spin(CUSTOMVERTEX* vertex,float spin_speed)
{
	D3DXToRadian(spin_speed);
	for (int i = 0; i < 4; i++)
	{
		CUSTOMVERTEX temp = vertex[i];
		vertex[i].x = temp.x * cos(spin_speed) - temp.y * sin(spin_speed);
		vertex[i].y = temp.x * sin(spin_speed) + temp.y * cos(spin_speed);
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
