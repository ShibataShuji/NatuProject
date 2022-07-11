#pragma once

#include <xaudio2.h>
#include "gameObject.h"


class Audio : public GameObject
{
private:
	static IXAudio2*				m_Xaudio;			// XAudioはウィンドウズに元から入っているのでライブライとかもいらない
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice;
	BYTE*					m_SoundData;

	int						m_Length;
	int						m_PlayLength;

	bool					m_Fadeout;
	float					m_Fadeout_StartVolume;
	float					m_Fadeout_NowVolume;
	float					m_Fadeout_Count_Max;
	float					m_Fadeout_Count;

	bool					m_Fadein;
	float					m_Fadein_StartVolume;
	float					m_Fadein_NowVolume;
	float					m_Fadein_Count_Max;
	float					m_Fadein_Count;


public:
	static void InitMaster();
	static void UninitMaster();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void Stop();
	void SetVolume(float volume);
	void SetFadeout(int FadeoutCount);
	void SetFadein(int FadeinCount);


};

