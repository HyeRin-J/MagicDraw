#include "../RRYEngine/framework.h"
#include "GameProc.h"
#include "System.h"
#include "SoundManager.h"

SoundManager* SoundManager::m_Instance = nullptr;

SoundManager* SoundManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new SoundManager();
	}

	return m_Instance;
}

void SoundManager::LoadData()
{

	TCHAR filePath[256];

	for (int i = 0; i < (int)BGMList::BGM_END; i++)
	{
		sprintf(filePath, "%s/%s", SOUND_PATH, BGMFileName[i]);
		m_BGM[i] = LPSOUNDEFFECT();
		ynSndLoad(filePath, &m_BGM[i]);
		ynSndSetVolume(m_BGM[i], 85);
	}
	for (int i = 0; i < (int)SoundList::SOUND_END; i++)
	{
		sprintf(filePath, "%s/%s", SOUND_PATH, MediaFileName[i]);
		m_Sounds[i] = LPSOUNDEFFECT();
		ynSndLoad(filePath, &m_Sounds[i]);
		ynSndSetVolume(m_Sounds[i], 90);
	}

	//효과음 볼륨 조절
	ynSndSetVolume(m_Sounds[(int)SoundList::BOSS_ATTACK], 100);
	ynSndSetVolume(m_Sounds[(int)SoundList::BOSS_SUMMON], 100);
	ynSndSetVolume(m_Sounds[(int)SoundList::BOSS_DIE], 100);
}

void SoundManager::PlayBackGround()
{
	ynSndStop(m_BGM[m_CurrMedia]);

	switch (StageInfo)
	{
	case Stage::title:
		m_CurrMedia = (int)BGMList::TITLE;
		break;
	case Stage::tutorial:
		m_CurrMedia = (int)BGMList::INGAME;
		break;
	case Stage::stageClear:
		m_CurrMedia = (int)BGMList::CLEAR;
		break;
	case Stage::introBoss:
		m_CurrMedia = (int)BGMList::BOSS;
		break;
	case Stage::gameover:
		m_CurrMedia = (int)BGMList::GAMEOVER;
		break;
	}

	ynSndPlay(m_BGM[m_CurrMedia], 1, 1);
}

void SoundManager::ReleaseData()
{
	for (int i = 0; i < (int)BGMList::BGM_END; i++)
	{
		ynSndStop(m_BGM[i]);
		ynSndRelease(m_BGM[i]);
	}
	
	for (int i = 0; i < (int)SoundList::SOUND_END; i++)
	{
		ynSndStop(m_Sounds[i]);
		ynSndRelease(m_Sounds[i]);
	}
	ynMMedia_Release();
}

void SoundManager::PlayOnce(SoundList list)
{
	ynSndPlay(m_Sounds[(int)list], FALSE, TRUE);
}