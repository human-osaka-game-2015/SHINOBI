#include "dx.render_lib.h"
#include <d3dx9math.h>

IDirect3DDevice9*	pD3Device;		//	Direct3D�̃f�o�C�X

D3DPRESENT_PARAMETERS d3dpp;		//	�p�����[�^�[




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
	//�`����@�̐ݒ�
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRC�̐ݒ�
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
	//��ʂ̏���
	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//�`��̊J�n
	pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	pD3Device->BeginScene();

}

void EndScene()
{
	//�`��̏I��
	pD3Device->EndScene();

	//�\��
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
		xfilename,				// x�t�@�C���̃t�@�C���p�X
		D3DXMESH_MANAGED,	// ���_�o�b�t�@�쐬�I�v�V����
		pD3Device,				// Direct3DDevice�|�C���^
		NULL,				// �אڐ��f�[�^�|���S����� �g��Ȃ�
		&pMatBuf,			// �}�e���A�����
		NULL,				// �G�t�F�N�g �g��Ȃ�
		&pThing->nMat,				// ���b�V���̐��擾
		&pThing->pMesh			// ���b�V�����̃|�C���^
		)))
	{
		MessageBox(NULL, "x�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", xfilename, MB_OK);
	}

	if (!(pThing->pMesh->GetFVF() & D3DFVF_NORMAL)) {

		ID3DXMesh* pTempMesh = NULL;

		pThing->pMesh->CloneMeshFVF(pThing->pMesh->GetOptions(),
			pThing->pMesh->GetFVF() | D3DFVF_NORMAL, pD3Device, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		pThing->pMesh->Release();
		pThing->pMesh = pTempMesh;
	}

	// �}�e���A���������o��
	D3DXMATERIAL*	d3Mat = (D3DXMATERIAL*)pMatBuf->GetBufferPointer();
	pThing->pMeshMat = new D3DMATERIAL9[pThing->nMat];		// ���b�V�������m��
	pThing->pMeshTex = new LPDIRECT3DTEXTURE9[pThing->nMat];// �e�N�X�`�����m��
	for (int i = 0; i < pThing->nMat; i++)
	{
		pThing->pMeshMat[i] = d3Mat[i].MatD3D;			// �}�e���A�����Z�b�g
		pThing->pMeshMat[i].Ambient = pThing->pMeshMat[i].Diffuse;// ����������
		pThing->pMeshTex[i] = NULL;	// �e�N�X�`��������

		// �g�p���Ă���e�N�X�`��������Γǂݍ���
		if (d3Mat[i].pTextureFilename != NULL &&
			lstrlen(d3Mat[i].pTextureFilename) > 0)
		{
			// �e�N�X�`���ǂݍ���
			if(FAILED(D3DXCreateTextureFromFile(
				pD3Device,
				d3Mat[i].pTextureFilename,
				&pThing->pMeshTex[i])))
			{
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}
	// �}�e���A�����J��
	pMatBuf->Release();



}

void Set_Transform(THING* pThing,float fScale)
{
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
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
	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	D3DXVECTOR3 vecEyePt(Eye_x, Eye_y, Eye_z); //�J�����i���_�j�ʒu
	D3DXVECTOR3 vecLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vecEyePt, &vecLookatPt, &vecUpVec);
	pD3Device->SetTransform(D3DTS_VIEW, &matView);
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 100.0f);
	pD3Device->SetTransform(D3DTS_PROJECTION, &matProj);
	// ���C�g�����Ă� ���F�ŋ��ʔ��˂���ɐݒ�
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
	// �����_�����O	 
	for (DWORD i = 0; i<pThing->nMat; i++)
	{
		pD3Device->SetMaterial(&pThing->pMeshMat[i]);
		pD3Device->SetTexture(0, pThing->pMeshTex[i]);
		pThing->pMesh->DrawSubset(i);
	}
}


void Transform_Draw_Thing(THING* pThing,float fScale)
{
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMATRIXA16 matWorld, matPosition, matScale;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPosition, pThing->vecPosition.x, pThing->vecPosition.y,pThing->vecPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

	D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
	pD3Device->SetTransform(D3DTS_WORLD, &matWorld);

	// �����_�����O	 
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
	// �����ŃG���[�������Ȃ��̂Ȃ�A�Ăь��ɂ��̂܂܃G���[�R�[�h�Ԃ����Ⴆ�΁H
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
