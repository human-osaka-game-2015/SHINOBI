#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#pragma once
/**
*@file byouga_lib.h
*@brief �`��֌W�̃��C�u����
*@author ���J�c���N
*/
extern IDirect3DDevice9*	pD3Device;		//	Direct3D�̃f�o�C�X



extern D3DPRESENT_PARAMETERS d3dpp;		//	�p�����[�^�[

typedef struct _THING
{
	LPD3DXMESH			pMesh;	// ���b�V���f�[�^
	DWORD				nMat;	// �}�e���A���̐�
	D3DMATERIAL9*			pMeshMat;	// �}�e���A�����
	LPDIRECT3DTEXTURE9*	pMeshTex;	// ���b�V���̃e�N�X�`��
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
 * @brief �摜��ǂݍ��ފ֐��̊g���łł��B
 *
 * @param pTexture
 * @param text �摜�̖��O
 * @param TexID ptexture�̔z��̗v�f��
 * @param alpha ���ߓx�̐ݒ� 255���ő�l
 * @param red �Ԃ̔䗦 255���ő�
 * @param green �΂̔䗦 255���ő�
 * @param blue �̔䗦 255���ő�
 *
 * @return ��������(���s�̏ꍇ�͕��̒l���Ԃ�)
 */
HRESULT Tex_Load_EX(LPDIRECT3DTEXTURE9 *pTexture,const char* text, int TexID, int alpha, int red, int green, int blue);


/**
 * @brief �摜��ǂݍ��ފ֐��ł��B
 *
 * @param pTexture
 * @param name �摜�̖��O
 * @param TexID ptexture�̔z��̗v�f��
 *
 * @return ��������(���s�̏ꍇ�͕��̒l���Ԃ�)
 */
HRESULT Tex_Load(LPDIRECT3DTEXTURE9 *pTexture,const char* name, int TexID);


/**
*@brief ��ʂ̏����ƃV�[���̊J�n
*/
void BeginScene();

/**
*@brief �V�[���̏I���A���ꂩ���ʂ֑���
*/
void EndScene();

/**
 * @brief �ǂݍ��񂾉摜��\������֐�
 *
 * @param pTexture
 * @param VertexName CUSTOMVERTEX�̖��O���|�C���^�œn��
 * @param	TexID
 */
void Tex_Draw(LPDIRECT3DTEXTURE9 *pTexture,CUSTOMVERTEX* VertexName, int TexID);

/**
*@brief �`����@�̐ݒ�
*/
void RenderSet(void);

/**
*@brief ��]�̊֐�
*@param *vertex CUSTOMVERTEX�̖��O���|�C���^�œn��
*@param spin_speed �}�C�i�X������]�E�v���X���E��]
*/
void Vertex_Spin(CUSTOMVERTEX* vertex, float spin_speed, CUSTOMVERTEX* temp);



/**
*@brief �摜���g�k����֐�
*@param *vertex CUSTOMVERTEX�̖��O���|�C���^�œn��
*@param x x���̊g�k�̔䗦
*@param y y���̊g�k�̔䗦
*/
void Vertex_Scaling(CUSTOMVERTEX *vertex, float x, float y);


/**
 * @brief
 *
 * @param xfilename x�t�@�C���̖��O������
 * @param pThing THING�^�̍\���̂�n��
 * @param pvecPosition �ǂݍ���x�t�@�C��x,y,z�̏����ʒu��ݒ肷��
 */
void Mesh_Load_FromX(LPTSTR xfilename, pTHING pThing, D3DXVECTOR3* pvecPosition);


/**
 * @brief ���[���h���W�ɕϊ�����
 *
 * @param pThing THING�^�̍\���̂̃|�C���^��n��
 */
void Set_Transform(THING* pThing, float fScale);


/**
 * @brief �J�����̍��W��ݒ肵�āA���C�g���I���ɂ���
 *
 * @param Eye_x �J������x���W��ݒ肷��
 * @param Eye_y �J������y���W��ݒ肷��
 * @param Eye_z �J������z���W��ݒ肷��
 */
void Set_View_Light(FLOAT Eye_x, FLOAT Eye_y, FLOAT Eye_z);


/**
 * @brief �ǂݍ��񂾃��b�V����\������
 *
 * @param pThing THING�^�̍\���̂̃|�C���^��n��
 */
void Draw_Thing(THING* pThing);

/**
* @brief ���[���h���W�ɕϊ����āA�ǂݍ��񂾃��b�V����\������B��{������g��
*
* @param pThing THING�^�̍\���̂̃|�C���^��n��
*/
void Transform_Draw_Thing(THING* pThing, float fScale, THING2D_POS* posxy);
