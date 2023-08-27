// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include <SDKDDKVer.h>
#include "ynMMedia.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
//#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

extern SIZE g_Screen;

//엔진 생성 및 릴리즈
int		CreateEngine(HWND hWnd, int screenWidth = 1920, int screenHeight = 1080, COLORREF bkColor = RGB(0, 0, 0));
void	ReleaseEngine();

//백버퍼 생성
int		CreateBackBuffer(HWND hWnd);
void	ReleaseBackBuffer();

//백버퍼에 렌더링 부분
int		BeginRendering();
int		EndRendering();
int		ClearBackBuffer(COLORREF col);
//화면 메인 버퍼에 백버퍼 출력
int		Flip();

//엔진 타이머 얻어오기
float	GetEngineTimer();
void	PrintFPS(int x, int y);

//텍스트 출력하기
int PrintText(int x, int y, COLORREF col, char* msg, ...);

struct SPRITE
{
	HBITMAP		bmp;		//비트맵 핸들
	HDC			dc;			//비트맵을 관리할 dc

	BITMAPINFO	bmpInfo;	//비트맵 정보
	int Width, Height;		//비트맵 크기
};
typedef SPRITE* PSPRITE;

struct ANISPRITE
{
	SPRITE spr = SPRITE();

	int cutX = 1, cutY = 1;	//스프라이트에 들어가는 애니메이션의 컷 수

	float aniTime = 0;
	int aniFrame = 0;

	int loopCount = -1;
	int currLoopCount = 0;
};
typedef ANISPRITE* PANISPRITE;

extern SPRITE g_BackBufferSprite;

int		LoadSprite(HWND hWnd, char* fileName, PSPRITE pSpr);
int		LoadAniSprite(HWND hWnd, char* fileName, int CutX, int CutY, PANISPRITE pSpr);
void	ReleaseSprite(SPRITE* pSpr);
void	ReleaseSprite(PSPRITE* pSpr);
void	ReleaseSprite(ANISPRITE* pSpr);
void	ReleaseSprite(PANISPRITE* pSpr);



void	DrawSprite(HDC hdc, int x, int y, PSPRITE spr, int offsetX = 0, int offSetY = 0, float ScaleX = 1, float ScaleY = 1);
void	DrawTransSprite(HDC hdc, int x, int y, PSPRITE pSpr, float ScaleX = 1, float ScaleY = 1, COLORREF transColor = RGB(255, 0, 255));
void	DrawAlphaSprite(HDC hdc, int x, int y, int w, int h, PSPRITE pSpr, int sprX = 0, int sprY = 0, float alpha = 1, float ScaleX = 1, float ScaleY = 1);
bool	DrawAnimation(float dTime, int aniFPS, int x, int y, PANISPRITE pSpr, float alpha = 1, float ScaleX = 1, float ScaleY = 1, COLORREF transColor = RGB(255, 0, 255));

// 스프라이트 회전 출력 :  지정위치에 스프라이트(텍스처)를 회전 출력한다. 
//                        출력시 회전 각도 필요 
//
void TextureDrawRot(HDC hdc,					//출력할 목표dc 핸들.
	int x, int y,				//출력 좌표.
	PSPRITE pSpr,			//출력할 스프라이트(텍스처) 포인터.
	float angle,				//회전량 (각도) 
	float scaleX = 1.0f,		//출력시 원본기준 스케일값 ( 100% = 1.0f )
	float scaleY = 1.0f,
	COLORREF transColor = RGB(255, 0, 255), 	//투명색.기본값은 (255, 0, 255)
	BOOL bDebug = FALSE						//디버그 설정 : 기본값은 FALSE. 디버그 모드에서는 스프라이트 회전 변환 영역(메모리DC) 을 녹색으로 표시하며, 투명색을 사용하지 않습니다.
);

void	DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF col, int width = 1);
void	DrawFillRect(HDC hdc, int x, int y, int width, int height, COLORREF fillCol, COLORREF lineCol = RGB(0, 0, 0), int lineWidth = 1);
void	DrawHollowRect(HDC hdc, int x, int y, int width, int height, COLORREF lineCol = RGB(0, 0, 0), int lineWidth = 1);

#define FPS(v)   (1.0f/(float)v)    

#define backBufferDC g_BackBufferSprite.dc

//릴리즈용
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#define SAFE_FREE(pBuff)    if((pBuff)){ free(pBuff); (pBuff) = NULL; }
#endif