#pragma once

#define SND_SPEED_DEFAULT 1.0f	   //�÷��� �ӵ� �⺻��=1.0 (100%)
#define SND_SPEED_MAX   4.0f       //5�� �̻��� ���� �鸮�� �ʴ´�.
#define SND_SPEED_MIN   0.1f	   //�ʹ� �����͵� ����. �����ϰ� ������ �Ǵ�.

#define MEDIA_MAX 8

#define SOUND_PATH "../Data/sounds/"

enum class BGMList
{
	TITLE, INGAME, BOSS, GAMEOVER, CLEAR, BGM_END
};

enum class SoundList
{
	MAGIC_LAUNCH, MAGIC_HIT, HERO_DAMAGE, BOSS_COMMING, BOSS_ATTACK, BOSS_SUMMON, BOSS_DIE, WAVE_CLEAR, SOUND_END
};

class SoundManager
{
	static SoundManager* m_Instance;

	LPSOUNDEFFECT m_BGM[(int)BGMList::BGM_END];
	LPSOUNDEFFECT m_Sounds[(int)SoundList::SOUND_END];

	float   m_PlaySpeed = 1.0f;		   //�÷��� �ӵ�. (����)��

	TCHAR* BGMFileName[(int)BGMList::BGM_END] =
	{
		"BGM_title.wav",
		"BGM_ingame.wav",
		"BGM_BossStage_02.wav",
		"GameOver.wav",
		"GameClear.wav"
	};

	TCHAR* MediaFileName[(int)SoundList::SOUND_END] =
	{	
		"MagicEffect_launch_02.wav",
		"MagicEffect_attack_02.wav",
		"Character_attacked_02.wav",
		"Monster_voice_coming.wav",
		"Monster_voice_attack.wav",
		"Monster_voice_recall.wav",
		"BossMonster_die.wav",
		"LevelUp_02.wav",
	};
	int    m_CurrMedia = 0;
	double m_CurrMediaPlayRate = 1.0f;
	//ynSndPlay(g_pSE1, PLAY_ONCE_, TRUE);	//1�� ����+�ǰ��� : Play �Լ� ȣ��ø��� 'ó��'���� �÷���.��
	//ynSndPlay(g_pSE2, PLAY_LOOP_, TRUE);	//���ѹݺ�+�ǰ���
	//ynSndStop(g_pSE2); //���� ����
	//ynSndVolumeUp(g_pSE2); //���� ��
	//ynSndVolumeDn(g_pSE2); //���� �ٿ�
	/*

		ynSndMasterVolumeUp(100);

		ynSndMasterVolumeDn(100);

		ynSndMasterSetVolume(SE_VOL_MAX_);		//100.

		ynSndMasterSetVolume(SE_VOL_MIN_);		//0.*/
	/*
	// ���� : MP3 �ε�.��
	// MP3 �� �ѹ��� �ϳ��� ���ϸ� �ε�, �÷��� ����. 
	// ���v BGM ���� ����� ����.
	ynMediaLoad( MediaFileName[CurrMedia] );	//��
	//ynMediaLoad("snd/thunder.mp3");			//��
	//ynMediaLoad("snd/GT2s.mp3");
	ynMediaPlay();*/

public:
	static SoundManager* GetInstance();

	void LoadData();
	void ReleaseData();

	void PlayBackGround();
	void PlayOnce(SoundList list);
};