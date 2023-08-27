#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "stdio.h"
#include "Unit.h"
#include "Hero.h"
#include "UnitManager.h"
#include "Monster.h"
#include "BossMonster.h"
#include "Input.h"
#include "Magic.h"
#include <vector>
#include "MagicManager.h"
#include "System.h"
#include "GameProc.h"
#include "SoundManager.h"

#pragma warning(disable:4996)

System* System::m_Instance = nullptr;

System::System()
{
	m_State = 0;

	m_DTime = 0.0f;
	m_DTimeSys = 0.0f;
	m_DTimeScale = 1.0f;
	m_DTimePlay = 0.0f;

	for (int i = 0; i < (int)BK_SPR_NUM::BK_SPR_END; i++)
	{
		m_BackgroundSprites[i] = SPRITE();
	}
}

System* System::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new System();
	}

	return m_Instance;
}

//������ �ε�... ��κ��� ��������Ʈ �ε��� ����
void System::DataLoading()
{
	//��׶��� �ε�
	char filePath[250];

	for (int i = 0; i < (int)BK_SPR_NUM::BK_SPR_END; i++)
	{
		sprintf(filePath, "%s/%s", RESOURCE_PATH, m_BackgroundPath[i]);
		LoadSprite(g_hWnd, filePath, &m_BackgroundSprites[i]);

	};

	//Ȥ�� �� �ӽ� ��������Ʈ��
	SPRITE temp;
	ANISPRITE aniTemp;

	sprintf(filePath, "%s/loading.bmp", RESOURCE_PATH);
	LoadAniSprite(g_hWnd, filePath, 4, 1, &m_LoadingSprite);

	//���ΰ� ��������Ʈ �ε�
	for (int i = 0; i < (int)CHAR_SPR_NUM::CHAR_SPR_END; i++)
	{
		sprintf(filePath, "%s/%s", CHARACTER_PATH, m_CharacterPath[i]);
		LoadAniSprite(g_hWnd, filePath, m_CharAniCutSize[i], 1, &aniTemp);
		memcpy(&UnitManager::GetInstance()->m_HeroSprites[i], &aniTemp, sizeof(ANISPRITE));
	}

	sprintf(filePath, "%s/Hp.bmp", CHARACTER_PATH);
	LoadSprite(g_hWnd, filePath, &temp);
	memcpy(&UnitManager::GetInstance()->m_HpSprite, &temp, sizeof(SPRITE));

	sprintf(filePath, "%s/warning.bmp", CHARACTER_PATH);
	LoadSprite(g_hWnd, filePath, &temp);
	memcpy(&UnitManager::GetInstance()->m_HeroHitSprite, &temp, sizeof(SPRITE));


	for (int i = 0; i < (int)MON_SPR_NUM::MON_SPR_END; i += 2)
	{
		sprintf(filePath, "%s/%s", MONSTER_PATH, m_MonsterPath[i]);
		LoadAniSprite(g_hWnd, filePath, m_MonAniCutSize[i], 1, &aniTemp);
		memcpy(&UnitManager::GetInstance()->m_MonsterAniSprites[i / 2], &aniTemp, sizeof(ANISPRITE));

		sprintf(filePath, "%s/%s", MONSTER_PATH, m_MonsterPath[i + 1]);
		LoadAniSprite(g_hWnd, filePath, m_MonAniCutSize[i + 1], 1, &aniTemp);
		memcpy(&UnitManager::GetInstance()->m_MonsterDeadAniSprite[i / 2], &aniTemp, sizeof(ANISPRITE));
	}

	sprintf(filePath, "%s/arrow.bmp", MONSTER_PATH);
	LoadSprite(g_hWnd, filePath, &temp);
	memcpy(&UnitManager::GetInstance()->m_ArrowSprite, &temp, sizeof(SPRITE));

	for (int i = 0; i < (int)BOSS_SPR_NUM::BOSS_SPR_END; i++)
	{
		sprintf(filePath, "%s/%s", BOSS_PATH, m_BossPath[i]);
		LoadAniSprite(g_hWnd, filePath, m_BossAniCutSize[i], 1, &aniTemp);
		memcpy(&UnitManager::GetInstance()->m_BossSprites[i], &aniTemp, sizeof(ANISPRITE));
	}

	for (int i = 0; i < 6; i++)
	{
		sprintf(filePath, "%s/eye_%d.bmp", BOSS_PATH, i + 1);
		LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
		memcpy(&UnitManager::GetInstance()->m_BossEyeSprite[i], &aniTemp, sizeof(ANISPRITE));
	}

	for (int i = 0; i < 9; i++)
	{
		sprintf(filePath, "%s/boss_hp/boss_hp%d.bmp", RESOURCE_PATH, i);
		LoadSprite(g_hWnd, filePath, &temp);
		memcpy(&UnitManager::GetInstance()->m_BossHpSprites[i], &temp, sizeof(SPRITE));
	}

	//����Ʈ �׽�Ʈ
	for (int i = 0; i < 4; i++)
	{
		sprintf(filePath, "%s/effect_%d.bmp", EFFECT_PATH, i + 1);
		LoadAniSprite(g_hWnd, filePath, 4, 1, &aniTemp);
		memcpy(&MagicManager::GetInstance()->m_MagicSprites[i], &aniTemp, sizeof(ANISPRITE));
	}

	sprintf(filePath, "%s/effect_explo.bmp", EFFECT_PATH);
	LoadAniSprite(g_hWnd, filePath, 2, 1, &aniTemp);
	memcpy(&MagicManager::GetInstance()->m_MagicExploSprite, &aniTemp, sizeof(ANISPRITE));

	//���� �׸� �� ȭ�鿡 ǥ������ ����Ʈ
	sprintf(filePath, "%s/Effect/magicEffect.bmp", RESOURCE_PATH);
	LoadSprite(g_hWnd, filePath, &temp);
	memcpy(&MagicManager::GetInstance()->m_MagicStarEffect, &temp, sizeof(SPRITE));

	CreateCustomFont();
}

void System::DataRelease()
{
	DeleteObject(m_HShapeBufferBmp);
	//SAFE_DELETE(m_HShapeBufferBmp);
	m_HShapeBufferBmp = nullptr;
	ReleaseDC(g_hWnd, m_ShapeBufferDC);
	DeleteObject(m_ShapeBufferDC);
	//SAFE_DELETE(m_ShapeBufferDC);
	m_ShapeBufferDC = nullptr;

	SelectObject(backBufferDC, m_OldFont);
	RemoveFontResource(m_fontPath);
	DeleteObject(m_SystemFont);
	m_SystemFont = nullptr;

	for (int i = 0; i < (int)BK_SPR_NUM::BK_SPR_END; i++)
	{
		ReleaseSprite(&m_BackgroundSprites[i]);

	}
	ReleaseSprite(&m_LoadingSprite);

	SAFE_DELETE(m_Instance);
}

void System::SystemUpdate()
{
#if _DEBUG
	if (IsKeyUp(VK_F1)) m_State ^= SHOWHELP;
	if (IsKeyUp(VK_F3)) m_State ^= SHOWFRM;		//Frm On/Off
	//�浹 �ڽ�.���.
	if (IsKeyUp(VK_F5)) m_State ^= SHOWBBOX;


	// Ÿ�̸� �ӵ� ����.
	if (IsKeyDown(VK_F7))									//�ð� ����
	{
		m_DTimeScale -= 1.0f * m_DTimeSys;				// 100%/s �� ����.. �׽�Ʈ1.
													//���Ӱ�� ���� ����..
		if (m_DTimeScale < 0.0001f) m_DTimeScale = 0.0001f;		//�׽�Ʈ.3
	}

	if (IsKeyDown(VK_F8))									//�ð� ����
	{
		m_DTimeScale += 1.0f * m_DTimeSys;				// 100%/s �� ��ȭ�ȴ�.. 
	}

#endif
	//���� �ý��� Ÿ��
	m_DTimeSys = GetEngineTimer();

	//�÷��� �ð� ����..
	m_DTimePlay += m_DTimeSys;

	// Ÿ�̸� #2 : �Ϲ� ���� Ÿ�̸� 
	m_DTime = m_DTimeSys * m_DTimeScale;

	// ���� ����...
	if (m_DTime < 0.0001f)	m_DTime = 0.0001f;
}

void System::ShowHelp()
{
	//�����Ӽ� ���.
	if (IsShowFrm())
		PrintFPS(0, 0);

	if (!IsShowHelp())
	{
		return;
	}

	int x = 700, y = 50;
	COLORREF black = RGB(0, 0, 0);

	//----------------------- 
	// ��Ÿ ����.
	//----------------------- 
	x = 0, y = 100;
	COLORREF red = RGB(255, 0, 0);
	PrintText(x, y += 32, red, "KillCount : %d", UnitManager::KillCount);

	x, y += 32 * 2;

	COLORREF col = RGB(0, 255, 255);
	COLORREF col2 = RGB(0, 255 * 0.7f, 255 * 0.7f);
	COLORREF col3 = RGB(255, 255, 0);
	PrintText(x, y += 32, m_State & SHOWHELP ? col : col2, "Help: F1 (%s)", m_State & SHOWHELP ? L"ON" : L"OFF");
	PrintText(x, y += 32, m_State & SHOWFRM ? col : col2, "Frm: F3 (%s)", m_State & SHOWFRM ? L"ON" : L"OFF");
	PrintText(x, y += 32, m_State & SHOWBBOX ? col : col2, "BBox: F5 (%s)", m_State & SHOWBBOX ? L"ON" : L"OFF");
	PrintText(x, y += 32, col, "Eng.Timer: F7/F8");

	y += 32 * 2;
	col = RGB(255, 255, 0);
	PrintText(x, y += 32, col, "[Timer]");
	PrintText(x, y += 32, col, "Eng : dTime=%.04f  Scale= %.2f%%", m_DTime, (double)m_DTimeScale * 100.0f);	// 1.0 �� 100% �� ȯ��ǥ��
	PrintText(x, y += 32, col, "PlayTime: %.1f ��", m_DTimePlay);									//�÷��� �ð�.(��)

	y += 32 * 2;
	PrintText(x, y += 32, col, "Shape: %d", m_ShapeNum);	//������ ����
}

bool System::ShakeBackGround()
{
	static bool isReachOffset = false;
	static float startTime = 0;
	static int offset = 0;

	if (!isReachOffset)
	{
		if (offset < 10)
		{
			startTime += m_DTime;
			offset += (10 * startTime) / 0.5f;
			DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::BACKGROUND], 0, 10 - offset);
		}
		else
		{
			isReachOffset = true;
			startTime = 0;
			offset = 0;
		}
		return false;
	}
	else
	{
		if (offset < 10)
		{
			startTime += m_DTime;
			offset += (10 * startTime) / 0.5f;
			DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::BACKGROUND], 0, offset);
		}
		else
		{
			isReachOffset = false;
			startTime = 0;
			offset = 0;
			return true;
		}
		return false;
	}
}

void System::RenderAll()
{
	BeginRendering();	//������ ����

	//��׶��� ���
	DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::BACKGROUND], 10, 10);

	//�Ϲ� ���������� ���� �������� �Ǵ�
	switch (StageInfo)
	{
	case Stage::title:
		m_BossGuideTimer = 0;
		m_State = 0;
		DrawSprite(backBufferDC, 0, 0, &System::GetInstance()->m_BackgroundSprites[(int)BK_SPR_NUM::TITLE]);
		break;
	case Stage::loading:
		DrawAnimation(m_DTime, 4, 0, 0, &m_LoadingSprite);
		break;

	case Stage::tutorial:
		DrawAlphaSprite(backBufferDC, 0, 0,
			m_BackgroundSprites[(int)BK_SPR_NUM::TUTORIAL].Width, m_BackgroundSprites[(int)BK_SPR_NUM::TUTORIAL].Height,
			&m_BackgroundSprites[(int)BK_SPR_NUM::TUTORIAL]);

		break;
	case Stage::stage:
		UnitManager::GetInstance()->DrawMonster();

		PrintText(550, 120, RGB(255, 255, 255), "WAVE %d / 3", WaveCount + 1);

		break;
	case Stage::stageClear:
	{
		DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::ENDING], 0, 0);
		DrawAlphaSprite(backBufferDC, 450, 200,
			m_BackgroundSprites[(int)BK_SPR_NUM::CLEAR].Width, m_BackgroundSprites[(int)BK_SPR_NUM::CLEAR].Height,
			&m_BackgroundSprites[(int)BK_SPR_NUM::CLEAR]);

		PrintText(520, 400, RGB(255, 255, 255), "Total Score : %d", g_Score);
	}
	break;
	case Stage::introBoss:
		UnitManager::GetInstance()->DrawBoss();
		UnitManager::GetInstance()->DrawMonster();
		DrawAlphaSprite(backBufferDC, 0, 0, m_ScreenSize.cx, m_ScreenSize.cy, &m_BackgroundSprites[(int)BK_SPR_NUM::INTROBOSS]);
		break;
	case Stage::boss:

		m_BossGuideTimer += m_DTime;

		if (UnitManager::GetInstance()->m_BossMonster->IsAttacking())
		{
			if (UnitManager::GetInstance()->m_BossMonster->m_AttackSprite->aniFrame >= 5)
			{
				ShakeBackGround();
				SoundManager::GetInstance()->PlayOnce(SoundList::BOSS_ATTACK);
				UnitManager::GetInstance()->m_IsHeroHit = true;
				if (UnitManager::GetInstance()->m_Hero->BossHit())
				{
					StageInfo = Stage::gameover;
				}
			}
		}

		if (!IsBossAttackStart())
		{
			DrawAlphaSprite(backBufferDC, 0, 0,
				m_BackgroundSprites[(int)BK_SPR_NUM::TUTORIAL_BOSS].Width, m_BackgroundSprites[(int)BK_SPR_NUM::TUTORIAL_BOSS].Height,
				&m_BackgroundSprites[(int)BK_SPR_NUM::TUTORIAL_BOSS]);
		}

		UnitManager::GetInstance()->DrawBoss();
		UnitManager::GetInstance()->DrawMonster();

		if (m_BossGuideTimer > 3.0f && UnitManager::GetInstance()->m_BossMonster->IsInitMoveFinish() && IsKeyDown(VK_LBUTTON))
			m_State |= BOSS_ATTACK_START;
		
		break;
	case Stage::gameover:
		DrawSprite(backBufferDC, 0, 0, &m_BackgroundSprites[(int)BK_SPR_NUM::ENDING], 0, 0);
		DrawAlphaSprite(backBufferDC, 430, 200,
			m_BackgroundSprites[(int)BK_SPR_NUM::GAME_OVER].Width, m_BackgroundSprites[(int)BK_SPR_NUM::GAME_OVER].Height,
			&m_BackgroundSprites[(int)BK_SPR_NUM::GAME_OVER]);

		PrintText(520, 400, RGB(255, 255, 255), "Total Score : %d", g_Score);
		break;
	default:
		break;
	}

	if (StageInfo == Stage::tutorial || StageInfo == Stage::stage || StageInfo == Stage::introBoss || StageInfo == Stage::boss)
	{
		DrawAlphaSprite(backBufferDC, 310, 0,
			m_BackgroundSprites[(int)BK_SPR_NUM::GUIDE].Width, m_BackgroundSprites[(int)BK_SPR_NUM::GUIDE].Height,
			&m_BackgroundSprites[(int)BK_SPR_NUM::GUIDE]);

		//���ΰ� �׸���
		UnitManager::GetInstance()->DrawHero();

		//������� ������ �׸���
		MagicManager::GetInstance()->DrawMagic();

		PrintText(1180, 20, RGB(0, 255, 255), "Score %4d", g_Score);
	}

	//���� ���
#if DEBUG_
	ShowHelp();
#endif

	//���� OR ����
	GdiTransparentBlt(backBufferDC, 0, 0, 1440, 900, m_ShapeBufferDC, 0, 0, 1440, 900, RGB(0, 0, 0));

	//���� ��
	EndRendering();

	//���� DC�� ���
	Flip();
}

void System::CreateCustomFont()
{
	AddFontResource(m_fontPath);

	m_SystemFont = CreateFont(45, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGEUL_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, DEFAULT_PITCH, "123RF");
	m_OldFont = (HFONT)SelectObject(backBufferDC, m_SystemFont);
}