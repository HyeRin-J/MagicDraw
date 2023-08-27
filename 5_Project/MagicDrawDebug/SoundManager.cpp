#include "../RRYEngine/framework.h"
#include "GameProc.h"
#include "System.h"
#include "SoundManager.h"

SoundManager* SoundManager::m_Instance= nullptr;

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

	for (int i = 0; i < MEDIA_MAX; i++)
	{
		sprintf(filePath, "%s%s", RESOURCE_PATH, MediaFileName[i]);

		ynSndLoad(filePath, m_Sounds[i]);
	}
}

void SoundManager::PlayBackGround()
{
	if (StageInfo == Stage::title)
	{
		ynSndStop(m_Sounds[m_CurrMedia]);
		m_CurrMedia = 0;
	}
	else
	{
		ynSndStop(m_Sounds[m_CurrMedia]);
		m_CurrMedia = 1;
	}

	ynSndPlay(m_Sounds[m_CurrMedia], 1, 1);
}

void SoundManager::PlayOnce(SoundList list)
{
	ynSndPlay(m_Sounds[(int)list], FALSE, TRUE);
}
