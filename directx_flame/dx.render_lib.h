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

//-------------------------------------------------------------
//
//	�`�揈��
//
//-------------------------------------------------------------


extern IDirect3DDevice9*	pD3Device;		//	Direct3D�̃f�o�C�X

extern D3DPRESENT_PARAMETERS d3dpp;		//	�p�����[�^�[

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
void Vertex_Spin(CUSTOMVERTEX* vertex, float spin_speed);



/**
*@brief �摜���g�k����֐�
*@param *vertex CUSTOMVERTEX�̖��O���|�C���^�œn��
*@param x x���̊g�k�̔䗦
*@param y y���̊g�k�̔䗦
*/
void Vertex_Scaling(CUSTOMVERTEX *vertex, float x, float y);
