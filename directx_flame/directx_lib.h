#pragma once

extern HWND hWnd;
extern IDirect3D9*			pDirect3D;		//	Direct3D�̃C���^�[�t�F�C�X
extern D3DDISPLAYMODE d3ddm;

/**
*@brief Directx�̏�����
*/
void DirectxInit();


/**
*@brief Directx�̃f�o�C�X�̍쐬
*/
void D3DCreate();