#pragma once

enum class BK_SPR_NUM
{
	TITLE, LOADING, BACKGROUND, INTROBOSS, ENDING, BK_SPR_END
};

class System
{
private:
	char* m_BackgroundPath[5] = {
		"title.bmp",
		"loading.bmp",
		"background.bmp",
		"introBoss.bmp",
		"ending.bmp",
	};

public:
	SPRITE m_BackgroundSprites[5];

	//����.
	COLORREF m_BkColor = RGB(237, 252, 255);
	SIZE m_ScreenSize = { 1440, 900 };
	char* m_WindowName = "Magic Draw!";

	bool  m_ShowHelp;
	bool  m_ShowDot;
	bool  m_ShowFrm;		//������ ���
	bool  m_GridOn;		//�׸��� ���
	bool  m_ShowBBox;		//�浹 �ڽ� ����.

	float m_DTime;		//�Ϲ� Ÿ�̸� : �Ϲ� ���ֿ�.
	float m_DTimeSys;		//�ý��� Ÿ�̸� : ���� ������ü �� ���ΰ���.
	float m_DTimeScale;	//���� �ӵ� ����..
	float m_DTimePlay;	//���� �÷��� �ð�.. 

	static System* m_Instance;
public:
	HFONT	m_SystemFont;

	HBITMAP m_HShapeBufferBmp = nullptr;		//����ۿ� '��' ��Ʈ���
	BITMAP  m_ShapeBufferInfo = BITMAP();
	HDC		m_ShapeBufferDC = nullptr;

	int m_ShapeNum = -1;

	System();

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
