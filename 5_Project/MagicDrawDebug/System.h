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

	//배경색.
	COLORREF m_BkColor = RGB(237, 252, 255);
	SIZE m_ScreenSize = { 1440, 900 };
	char* m_WindowName = "Magic Draw!";

	bool  m_ShowHelp;
	bool  m_ShowDot;
	bool  m_ShowFrm;		//프레임 출력
	bool  m_GridOn;		//그리드 출력
	bool  m_ShowBBox;		//충돌 박스 보기.

	float m_DTime;		//일반 타이머 : 일반 유닛용.
	float m_DTimeSys;		//시스템 타이머 : 엔진 정적객체 및 주인공용.
	float m_DTimeScale;	//엔진 속도 조절..
	float m_DTimePlay;	//게임 플레이 시간.. 

	static System* m_Instance;
public:
	HFONT	m_SystemFont;

	HBITMAP m_HShapeBufferBmp = nullptr;		//백버퍼용 '빈' 비트멥★
	BITMAP  m_ShapeBufferInfo = BITMAP();
	HDC		m_ShapeBufferDC = nullptr;

	int m_ShapeNum = -1;

	System();

	static System* GetInstance();

	void DataLoading();			//데이터 로딩 함수
	void DataRelease();			//데이터 제거

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
// 1/1000 초로 동기화 합니다.
#define  FPS(v)   (1.0f/(float)v)     
