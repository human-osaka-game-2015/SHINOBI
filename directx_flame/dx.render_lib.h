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
extern IDirect3DDevice9*	pD3Device;		//	Direct3Dのデバイス



extern D3DPRESENT_PARAMETERS d3dpp;		//	パラメーター

typedef struct _THING
{
	LPD3DXMESH			pMesh;	// メッシュデータ
	DWORD				nMat;	// マテリアルの数
	D3DMATERIAL9*			pMeshMat;	// マテリアル情報
	LPDIRECT3DTEXTURE9*	pMeshTex;	// メッシュのテクスチャ
	D3DXVECTOR3 vecPosition;
}THING,*pTHING;

typedef struct THING_POS
{
	FLOAT thing_posx;
	FLOAT thing_posy;
}THING2D_POS;

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
void Vertex_Spin(CUSTOMVERTEX* vertex, float spin_speed, CUSTOMVERTEX* temp);



/**
*@brief 画像を拡縮する関数
*@param *vertex CUSTOMVERTEXの名前をポインタで渡す
*@param x x軸の拡縮の比率
*@param y y軸の拡縮の比率
*/
void Vertex_Scaling(CUSTOMVERTEX *vertex, float x, float y);


/**
 * @brief
 *
 * @param xfilename xファイルの名前を入れる
 * @param pThing THING型の構造体を渡す
 * @param pvecPosition 読み込んだxファイルx,y,zの初期位置を設定する
 */
void Mesh_Load_FromX(LPTSTR xfilename, pTHING pThing, D3DXVECTOR3* pvecPosition);


/**
 * @brief ワールド座標に変換する
 *
 * @param pThing THING型の構造体のポインタを渡す
 */
void Set_Transform(THING* pThing, float fScale);


/**
 * @brief カメラの座標を設定して、ライトをオンにする
 *
 * @param Eye_x カメラのx座標を設定する
 * @param Eye_y カメラのy座標を設定する
 * @param Eye_z カメラのz座標を設定する
 */
void Set_View_Light(FLOAT Eye_x, FLOAT Eye_y, FLOAT Eye_z);


/**
 * @brief 読み込んだメッシュを表示する
 *
 * @param pThing THING型の構造体のポインタを渡す
 */
void Draw_Thing(THING* pThing);

/**
* @brief ワールド座標に変換して、読み込んだメッシュを表示する。基本これを使う
*
* @param pThing THING型の構造体のポインタを渡す
*/
void Transform_Draw_Thing(THING* pThing, float fScale, THING2D_POS* posxy);
