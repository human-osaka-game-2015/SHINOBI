#include "d_sound.h"

TCHAR gName[100] = _T("DirectSound Ultra Short Basical Program");
IDirectSoundBuffer8 *pDSBuffer[MAX_SOUND];
IDirectSound8 *pDS8;

bool sound_flag[MAX_SOUND]{0};

bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
	if (filepath == 0)
		return false;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
	if (!hMmio)
		return false; // ファイルオープン失敗

	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
	if (size != fmsize) {
		mmioClose(hMmio, 0);
		return false;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
		return false;
	}
	*ppData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
	if (size != dataChunk.cksize) {
		delete[] * ppData;
		return false;
	}
	dataSize = size;

	// ハンドルクローズ
	mmioClose(hMmio, 0);

	return true;
}
//
//LRESULT CALLBACK Wnd_Proc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam){
//	if (mes == WM_DESTROY) { PostQuitMessage(0); return 0; }
//	return DefWindowProc(hWnd, mes, wParam, lParam);
//}

int _Sound_input(char* name, HWND hWnd, SOUND s_name)
{
	/*MSG msg; HWND hWnd;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, Wnd_Proc, 0, 0, hInstance, NULL, NULL,
		(HBRUSH)(COLOR_WINDOW + 1), NULL, (TCHAR*)gName, NULL };
	if (!RegisterClassEx(&wcex)) return 0;

	if (!(hWnd = CreateWindow(gName, gName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, NULL)))
		return 0;*/

	// Waveファイルオープン
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;
	if (!openWave((name), wFmt, &pWaveData, waveSize))
		return 0;

	// サウンドデバイス作成
	//IDirectSound8 *pDS8;
	DirectSoundCreate8(NULL, &pDS8, NULL);
	pDS8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = 0;
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;
	
	IDirectSoundBuffer *ptmpBuf = 0;
	
	pDS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer[s_name]);
	ptmpBuf->Release();
	if (pDSBuffer[s_name] == 0) {
		pDS8->Release();
		return 0;
	}

	// セカンダリバッファにWaveデータ書き込み
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if (DS_OK == pDSBuffer[s_name]->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
		memcpy(lpvWrite, pWaveData, dwLength);
		pDSBuffer[s_name]->Unlock(lpvWrite, dwLength, NULL, 0);
	}
	delete[] pWaveData; // 元音はもういらない
	return 0;
}

void play_back_BGM(SOUND s_name)
{
	if (sound_flag[s_name] == 0)
	{
		pDSBuffer[s_name]->Play(0, 0, DSBPLAY_LOOPING);
		sound_flag[s_name] = 1;
	}
}

void play_back_SE(SOUND s_name)
{
	//if (sound_flag[s_name] == 0)
	{
		pDSBuffer[s_name]->Play(0, 0, 0);
		sound_flag[s_name] = 1;
	}
}

void sound_stop(SOUND s_name)
{
	if (sound_flag[s_name] == 1)
	{
		pDSBuffer[s_name]->Stop();
		pDSBuffer[s_name]->SetCurrentPosition(0);
		sound_flag[s_name] = 0;
	}
}

void sound_release()
{
	pDS8->Release();
}