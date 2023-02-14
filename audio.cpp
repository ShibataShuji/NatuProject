
#include "stdafx.h"

//#include <cassert>
//#include "main.h"
//#include "audio.h"
//#include "gameObject.h"
//#include <xaudio2.h>





IXAudio2*				Audio::m_Xaudio = NULL;
IXAudio2MasteringVoice*	Audio::m_MasteringVoice = NULL;


void Audio::InitMaster()
{
	// XAudioを初期化する処理
	// COM初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio生成
	XAudio2Create(&m_Xaudio, 0);

	// マスタリングボイス生成
	m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);
}


void Audio::UninitMaster()
{
	// XAudioを終了する処理
	m_MasteringVoice->DestroyVoice();
	m_Xaudio->Release();
	CoUninitialize();
}









void Audio::Load(const char *FileName)
{

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		m_SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)m_SoundData, buflen);


		m_Length = readlen;
		m_PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	m_Xaudio->CreateSourceVoice(&m_SourceVoice, &wfx);
	assert(m_SourceVoice);
}


void Audio::Init()
{
	m_Fadeout_Count = 0;
	m_Fadeout = false;
}

void Audio::Uninit()
{
	m_SourceVoice->Stop();
	m_SourceVoice->DestroyVoice();

	delete[] m_SoundData;
}

void Audio::Update()
{
	if (m_Fadeout)
	{
		m_Fadeout_Count++;
		float ratio = (m_Fadeout_Count_Max - m_Fadeout_Count) / m_Fadeout_Count_Max;
		m_Fadeout_NowVolume = m_Fadeout_StartVolume * ratio;

		m_SourceVoice->SetVolume(m_Fadeout_NowVolume);
		if (m_Fadeout_Count > m_Fadeout_Count_Max)
		{

			m_Fadeout_Count = 0;
			m_Fadeout_Count_Max = 0;
			m_Fadeout = false;
			Audio::Stop();
		}
	}
	if (m_Fadein)
	{
		if (m_Fadein_Count == 0)
		{
			Audio::Play();
		}
		m_Fadein_Count++;
		float ratio = m_Fadein_Count_Max / (m_Fadein_Count_Max - m_Fadein_Count);
		m_Fadein_NowVolume = m_Fadein_StartVolume * ratio;

		m_SourceVoice->SetVolume(m_Fadein_NowVolume);
		if (m_Fadein_Count > m_Fadein_Count_Max)
		{

			m_Fadein_Count = 0;
			m_Fadein_Count_Max = 0;
			m_Fadein = false;
		}
	}

}

void Audio::Draw()
{

}





void Audio::Play(bool Loop)
{
	m_SourceVoice->Stop();
	m_SourceVoice->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_Length;
	bufinfo.pAudioData = m_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_PlayLength;

	// ループ設定
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = m_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);


	//float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	//m_SourceVoice->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
	////m_SourceVoice->SetVolume(0.1f);



	// 再生
	m_SourceVoice->Start();

}



void Audio::Stop()
{
	m_SourceVoice->Stop();
	m_SourceVoice->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_Length;
	bufinfo.pAudioData = m_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_PlayLength;

	m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

}

void Audio::SetVolume(float volume)
{
	m_SourceVoice->SetVolume(volume);
}



void Audio::SetFadeout(int FadeoutCount)
{
	m_Fadeout_Count_Max = FadeoutCount;
	m_Fadeout = true;
	float* p_m_Fadeout_StartVolume = &m_Fadeout_StartVolume;
	m_SourceVoice->GetVolume(p_m_Fadeout_StartVolume);
}

void Audio::SetFadein(int FadeinCount)
{
	m_Fadein_Count_Max = FadeinCount;
	m_Fadein = true;
	float* p_m_Fadein_StartVolume = &m_Fadein_StartVolume;
	m_SourceVoice->GetVolume(p_m_Fadein_StartVolume);
}
