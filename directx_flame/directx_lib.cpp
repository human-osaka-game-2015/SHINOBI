#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

#include "dx.render_lib.h"
#include "directx_lib.h"

D3DDISPLAYMODE d3ddm;
IDirect3D9*			pDirect3D;
HWND hWnd;

void DirectxInit()
{
	//DirectX オブジェクトの生成
	pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);

	//Display Mode の設定
	pDirect3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&d3ddm);

	ZeroMemory(&d3dpp,
		sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

}


void D3DCreate()
{
	//デバイスを作る
	pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &pD3Device);

}
