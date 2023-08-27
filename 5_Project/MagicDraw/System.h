#pragma once

enum class BK_SPR_NUM
{
	TITLE, BACKGROUND, INTROBOSS, ENDING, CLEAR, GAME_OVER, GUIDE, TUTORIAL, TUTORIAL_BOSS, BK_SPR_END
};

enum class CHAR_SPR_NUM
{
	IDLE, ATTACK, DAMAGE, BOSS_DAMAGE, GAME_CLEAR, CHAR_SPR_END
};

enum class MON_SPR_NUM
{
	BUG_WALK, BUG_DEAD, LEAF_WALK, LEAF_DEAD, MUSH_WALK, MUSH_DEAD, GOLEM_WALK, GOLEM_DEAD, MON_SPR_END
};

enum class BOSS_SPR_NUM
{
	WALK, DEAD, ATTACK, SUMMON, DAMAGE, BOSS_SPR_END
};

///���� ����
#define SHOWHELP	0x0001
#define SHOWFRM		0x0010
#define SHOWBBOX	0x0100
#define BOSS_ATTACK_START 0x1000

class System
{
private:
	char*	m_BackgroundPath[(int)BK_SPR_NUM::BK_SPR_END] = {
				"title.bmp",
				"background.bmp",
				"introBoss.bmp",
				"ending.bmp",
				"clear.bmp",
				"gameOver.bmp",
				"guide.bmp",
				"tutorial.bmp",
				"tutorial_boss.bmp"
			};

	char*	m_CharacterPath[(int)CHAR_SPR_NUM::CHAR_SPR_END] = {
				"standing.bmp",
				"attack.bmp",
				"damage.bmp",
				"bossdamage.bmp",
				"jumping.bmp"
			};
	int		m_CharAniCutSize[(int)CHAR_SPR_NUM::CHAR_SPR_END] = { 5, 3, 3, 16, 5 };

	char*	m_MonsterPath[(int)MON_SPR_NUM::MON_SPR_END] = {
				"bug_ani_glow.bmp",
				"bug_ouch.bmp",
				"leaf_ani_glow.bmp",
				"leaf_ani_ouch.bmp",
				"m_mushroom_ani_glow.bmp",
				"mushroom_ouch.bmp",
				"m_golem_ani_glow.bmp",
				"golem_ouch.bmp",
			};
	int		m_MonAniCutSize[(int)MON_SPR_NUM::MON_SPR_END] = { 6, 2, 6, 2, 14, 2, 9, 2 };
	
	char*	m_BossPath[(int)BOSS_SPR_NUM::BOSS_SPR_END] = {
				"boss_walk.bmp",
				"boss_dead.bmp",
				"boss_attack.bmp",
				"boss_mouth.bmp",
				"boss_ouch.bmp",
			};
	int		m_BossAniCutSize[(int)BOSS_SPR_NUM::BOSS_SPR_END] = { 4, 9, 6, 4, 2};
	char*	m_fontPath = "../Data/Font/123RF.ttf";
public:
	static System* m_Instance;

	SPRITE m_BackgroundSprites[(int)BK_SPR_NUM::BK_SPR_END];
	ANISPRITE m_LoadingSprite;

	//����.
	COLORREF m_BkColor = RGB(237, 252, 255);
	SIZE	m_ScreenSize = { 1440, 900 };
	char*	m_WindowName = "Magic Draw!";

	int		m_State = 0x0000;

	float	m_DTime;		//�Ϲ� Ÿ�̸� : �Ϲ� ���ֿ�.
	float	m_DTimeSys;		//�ý��� Ÿ�̸� : ���� ������ü �� ���ΰ���.
	float	m_DTimeScale;	//���� �ӵ� ����..
	float	m_DTimePlay;	//���� �÷��� �ð�.. 
	float	m_BossGuideTimer; //���� ���̵� Ÿ�̸�
public:
	HFONT	m_SystemFont = nullptr, m_OldFont = nullptr;

	HBITMAP m_HShapeBufferBmp = nullptr;		//����ۿ� '��' ��Ʈ���
	BITMAP  m_ShapeBufferInfo = BITMAP();
	HDC		m_ShapeBufferDC = nullptr;

	int m_ShapeNum = -1;

	System();

	bool IsShowHelp() { return m_State & SHOWHELP; }
	bool IsShowFrm() { return m_State & SHOWFRM; }
	bool IsShowBBox() { return m_State & SHOWBBOX; }
	bool IsBossAttackStart() { return m_State & BOSS_ATTACK_START; }

	static System* GetInstance();

	void DataLoading();			//������ �ε� �Լ�
	void DataRelease();			//������ ����

	void SystemUpdate();
	void ShowHelp();
	bool ShakeBackGround();
	void RenderAll();

	void CreateCustomFont();
};

extern HWND g_hWnd;

#define RESOURCE_PATH "../Data/"
#define CHARACTER_PATH "../Data/character_ani/"
#define MONSTER_PATH "../Data/monster/"
#define EFFECT_PATH "../Data/Effect/"
#define BOSS_PATH "../Data/boss/"

///////////////////////////////////////////////////////////////////////////////
// 1/1000 �ʷ� ����ȭ �մϴ�.
#define  FPS(v)   (1.0f/(float)v)     
