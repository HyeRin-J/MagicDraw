#pragma once

#define SND_SPEED_DEFAULT 1.0f	   //�÷��� �ӵ� �⺻��=1.0 (100%)
#define SND_SPEED_MAX   4.0f       //5�� �̻��� ���� �鸮�� �ʴ´�.
#define SND_SPEED_MIN   0.1f	   //�ʹ� �����͵� ����. �����ϰ� ������ �Ǵ�.

#define MEDIA_MAX 8

enum class SoundList
{
	MAGIC_LAUNCH = 2, MAGIC_HIT, BOSS_COMMING, BOSS_SUMMON, BOSS_GROWL, BOSS_DIE
};

class SoundManager
{
	static SoundManager* m_Instance;

	LPSOUNDEFFECT m_Sounds[MEDIA_MAX];


	float   m_PlaySpeed = 1.0f;		   //�÷��� �ӵ�. (����)��

	TCHAR* MediaFileName[MEDIA_MAX] =
	{
		"sounds/BGM_title.wav",
		"sounds/BGM_ingame.wav",
		"sounds/MagicEffect_launch_02.wav",
		"sounds/MagicEffect_attack_02.wav",
		"sounds/Monster_voice_coming.wav",
		"sounds/BossMonster_monstercall.wav",
		"sounds/BossMonster_growl.wav",
		"sounds/BossMonster_die.wav",
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

	void PlayBackGround();
	void PlayOnce(SoundList list);
};