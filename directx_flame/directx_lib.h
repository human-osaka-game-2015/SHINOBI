#pragma once

extern HWND hWnd;
extern IDirect3D9*			pDirect3D;		//	Direct3Dのインターフェイス
extern D3DDISPLAYMODE d3ddm;

/**
*@brief Directxの初期化
*/
void DirectxInit();


/**
*@brief Directxのデバイスの作成
*/
void D3DCreate();