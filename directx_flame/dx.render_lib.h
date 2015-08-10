#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma once
/**
*@file byouga_lib.h
*@brief 描画関係のライブラリ
*@author 小谷慶太朗
*/

//-------------------------------------------------------------
//
//	描画処理
//
//-------------------------------------------------------------


extern IDirect3DDevice9*	pD3Device;		//	Direct3Dのデバイス

extern D3DPRESENT_PARAMETERS d3dpp;		//	パラメーター

struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/**
 * @brief 画像を読み込む関数の拡張版です。
 *
 * @param pTexture
 * @param text 画像の名前
 * @param TexID ptextureの配列の要素数
 * @param alpha 透過度の設定 255が最大値
 * @param red 赤の比率 255が最大
 * @param green 緑の比率 255が最大
 * @param blue 青の比率 255が最大
 *
 * @return 処理結果(失敗の場合は負の値が返る)
 */
HRESULT Tex_Load_EX(LPDIRECT3DTEXTURE9 *pTexture,const char* text, int TexID, int alpha, int red, int green, int blue);


/**
 * @brief 画像を読み込む関数です。
 *
 * @param pTexture  
 * @param name 画像の名前
 * @param TexID ptextureの配列の要素数
 *
 * @return 処理結果(失敗の場合は負の値が返る)
 */
HRESULT Tex_Load(LPDIRECT3DTEXTURE9 *pTexture,const char* name, int TexID);


/**
*@brief 画面の消去とシーンの開始
*/
void BeginScene();

/**
*@brief シーンの終了、それから画面へ送る
*/
void EndScene();

/**
 * @brief 読み込んだ画像を表示する関数
 *
 * @param pTexture
 * @param VertexName CUSTOMVERTEXの名前をポインタで渡す
 * @param	TexID
 */
void Tex_Draw(LPDIRECT3DTEXTURE9 *pTexture,CUSTOMVERTEX* VertexName, int TexID);

/**
*@brief 描画方法の設定
*/
void RenderSet(void);

/**
*@brief 回転の関数
*@param *vertex CUSTOMVERTEXの名前をポインタで渡す
*@param spin_speed マイナスが左回転・プラスが右回転
*/
void Vertex_Spin(CUSTOMVERTEX* vertex, float spin_speed);



/**
*@brief 画像を拡縮する関数
*@param *vertex CUSTOMVERTEXの名前をポインタで渡す
*@param x x軸の拡縮の比率
*@param y y軸の拡縮の比率
*/
void Vertex_Scaling(CUSTOMVERTEX *vertex, float x, float y);
