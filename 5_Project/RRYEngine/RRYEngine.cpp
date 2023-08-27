// RRYEngine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//
#include "framework.h"
#include "stdio.h"
#include "Vector.h"
#include "math.h"

//윈도우 핸들
extern HWND g_hWnd;

//백버퍼 생성
SPRITE g_BackBufferSprite;
COLORREF g_BackColor = RGB(237, 252, 255);

SIZE g_Screen = { 1440, 900 };

int CreateEngine(HWND hWnd, int screenWidth, int screenHeight, COLORREF backColor)
{
	g_Screen.cx = screenWidth;
	g_Screen.cy = screenHeight;

	g_BackColor = backColor;

	CreateBackBuffer(hWnd);	//백버퍼 생성

	return 1;
}

void ReleaseEngine()
{
	ReleaseBackBuffer();
	return;
}

int CreateBackBuffer(HWND hWnd)
{
	SPRITE tmp;
	HDC tDC = GetDC(hWnd);

	//빈 비트맵 & 메모리 DC 생성
	tmp.bmp = CreateCompatibleBitmap(tDC, g_Screen.cx, g_Screen.cy);
	tmp.dc = CreateCompatibleDC(tDC);
	SelectObject(tmp.dc, tmp.bmp);

	HBRUSH bkBrush = (HBRUSH)CreateSolidBrush(g_BackColor);
	HBRUSH oldBrush = (HBRUSH)SelectObject(tmp.dc, bkBrush);

	Rectangle(tmp.dc, 0, 0, g_Screen.cx, g_Screen.cy);

	SelectObject(tmp.dc, oldBrush);
	DeleteObject(bkBrush);

	ReleaseDC(hWnd, tDC);

	g_BackBufferSprite = tmp;

	return true;
}

void ReleaseBackBuffer()
{
	DeleteObject(g_BackBufferSprite.bmp);
	g_BackBufferSprite.bmp = NULL;

	DeleteObject(g_BackBufferSprite.dc);
	g_BackBufferSprite.dc = NULL;
}

int BeginRendering()
{
	//렌더링 할 DC, 즉 백버퍼의 DC
	SetBkMode(g_BackBufferSprite.dc, TRANSPARENT);
	Rectangle(g_BackBufferSprite.dc, 0, 0, 1440, 900);

	return 0;
}

//그냥 끝났음을 알려줌
int EndRendering()
{
	return 0;
}

int ClearBackBuffer(COLORREF col)
{
	HBRUSH newBrush = CreateSolidBrush(col);
	RECT rc = { 0, 0, g_Screen.cx, g_Screen.cy };

	FillRect(backBufferDC, &rc, newBrush);

	DeleteObject(newBrush);

	g_BackColor = col;

	return 1;
}

int Flip()
{
	HDC mainDC = GetDC(g_hWnd);

	BitBlt(mainDC, 0, 0, g_Screen.cx, g_Screen.cy, backBufferDC, 0, 0, SRCCOPY);

	ReleaseDC(g_hWnd, mainDC);

	return 1;
}

float GetEngineTimer()
{
	static int oldTime = GetTickCount64();
	int nowTime = GetTickCount64();
	float dTime = ((nowTime - oldTime) % 1000) * 0.001f;
	oldTime = nowTime;

	return dTime;
}

//1초간 증가한 fps 출력
void PrintFPS(int x, int y)
{
	static int frameRate = 0;
	static UINT oldTime = GetTickCount64();
	static UINT frameCnt = 0;
	static float fps = 0.0f;

	++frameCnt;

	int time = GetTickCount64() - oldTime;

	// 1000밀리초 = 1초
	if (time >= 999)
	{
		oldTime = GetTickCount64();

		//1초간 증가한 프레임 수
		frameRate = frameCnt;
		frameCnt = 0;

		//초당 프레임 수
		fps = (float)(frameRate * 1000) / (float)time;
	}
	char msg[80];
	sprintf_s(msg, "fps = %.1f / %d ", fps, time);
	PrintText(x, y, RGB(0, 255, 0), msg);
}

int PrintText(int x, int y, COLORREF col, char* msg, ...)
{
	RECT rc = { x, y, x + 800, y + 600 };
	char mes[256];
	va_list va;
	va_start(va, msg);

	vsprintf_s(mes, sizeof(mes), msg, va);

	va_end(va);

	SetTextColor(backBufferDC, col);
	int result = DrawText(backBufferDC, mes, strlen(mes), &rc, DT_WORDBREAK);
	SetTextColor(backBufferDC, RGB(0, 0, 0));

	return result;
}

int LoadSprite(HWND hWnd, char* fileName, PSPRITE pSpr)
{
	if (pSpr == NULL) return false;

	SPRITE tmp;
	HDC mainDC = GetDC(hWnd);

	//비트맵 로딩
	tmp.bmp = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	//메모리 DC에 비트맵을 선택
	tmp.dc = CreateCompatibleDC(mainDC);
	SelectObject(tmp.dc, tmp.bmp);

	ReleaseDC(hWnd, mainDC);

	ZeroMemory(&tmp.bmpInfo, sizeof(BITMAPINFO));
	DIBSECTION ds;
	GetObject(tmp.bmp, sizeof(ds), &ds);
	tmp.bmpInfo.bmiHeader = ds.dsBmih;		//로딩된, 비트멥 정보 전체 획득.	
	tmp.Width = ds.dsBmih.biWidth;			//자주 사용되는 크기를 별도로 저장.
	tmp.Height = ds.dsBmih.biHeight;

	//외부데이터에 복사.
	*pSpr = tmp;

	return true;
}

int LoadAniSprite(HWND hWnd, char* fileName, int CutX, int CutY, PANISPRITE pSpr)
{
	if (pSpr == NULL) return false;

	SPRITE tmp;
	HDC mainDC = GetDC(hWnd);

	//비트맵 로딩
	tmp.bmp = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	//메모리 DC에 비트맵을 선택
	tmp.dc = CreateCompatibleDC(mainDC);

	SelectObject(tmp.dc, tmp.bmp);


	ZeroMemory(&tmp.bmpInfo, sizeof(BITMAPINFO));
	DIBSECTION ds;
	GetObject(tmp.bmp, sizeof(ds), &ds);
	tmp.bmpInfo.bmiHeader = ds.dsBmih;		//로딩된, 비트멥 정보 전체 획득.	
	tmp.Width = ds.dsBmih.biWidth;			//자주 사용되는 크기를 별도로 저장.
	tmp.Height = ds.dsBmih.biHeight;

	//외부데이터에 복사.
	pSpr->spr = tmp;
	pSpr->spr.Width = tmp.Width / CutX;
	pSpr->spr.Height = tmp.Height / CutY;

	//TransparentBlt(pSpr->spr.dc, 0, 0, tmp.Width, tmp.Height, tmp.dc, 0, 0, tmp.Width, tmp.Height, RGB(255, 0, 255));

	pSpr->cutX = CutX;
	pSpr->cutY = CutY;

	ReleaseDC(hWnd, mainDC);
	return true;
}

void ReleaseSprite(SPRITE* pSpr)
{
	if (pSpr != nullptr)
	{
		DeleteObject(pSpr->bmp);
		pSpr->bmp = nullptr;

		DeleteDC(pSpr->dc);
		pSpr->dc = nullptr;
	}
}

void ReleaseSprite(PSPRITE* pSpr)
{
	if (*pSpr != nullptr)
	{
		DeleteObject((*pSpr)->bmp);
		(*pSpr)->bmp = nullptr;

		DeleteDC((*pSpr)->dc);
		(*pSpr)->dc = nullptr;
	}
	SAFE_DELETE(*pSpr);
	*pSpr = nullptr;
}

void ReleaseSprite(ANISPRITE* pSpr)
{
	if (pSpr != nullptr)
	{
		DeleteObject(pSpr->spr.bmp);
		pSpr->spr.bmp = nullptr;

		DeleteDC(pSpr->spr.dc);
		pSpr->spr.dc = nullptr;
	}
}

void ReleaseSprite(PANISPRITE* pSpr)
{
	if (*pSpr != nullptr)
	{
		DeleteObject((*pSpr)->spr.bmp);
		(*pSpr)->spr.bmp = nullptr;

		DeleteDC((*pSpr)->spr.dc);
		(*pSpr)->spr.dc = nullptr;
	}
	SAFE_DELETE(*pSpr);
	*pSpr = nullptr;
}

void DrawSprite(HDC hdc, int x, int y, PSPRITE pSpr, int offSetX, int offSetY, float scaleX, float scaleY)
{
	//스케일 처리..
	int width = (int)(pSpr->Width * scaleX);
	int height = (int)(pSpr->Height * scaleY);

	//투명색 없이 그리기.
	StretchBlt(hdc, x, y, width, height, pSpr->dc, offSetX, offSetY, pSpr->Width, pSpr->Height, SRCCOPY);
}

void DrawTransSprite(HDC hdc, int x, int y, PSPRITE pSpr, float scaleX, float scaleY, COLORREF transColor)
{
	//스케일 처리..
	int width = (int)(pSpr->Width * scaleX);
	int height = (int)(pSpr->Height * scaleY);

	//투명색 처리.
	GdiTransparentBlt(hdc, x, y, width, height,
		pSpr->dc, 0, 0, pSpr->Width, pSpr->Height,  //<-- 필요하다면, 일정 부분 잘라서 그리기도 가능하겠지요.
		transColor);
}

void DrawAlphaSprite(HDC hdc, int x, int y, int w, int h, PSPRITE pSpr, int sprX, int sprY, float alpha, float scaleX, float scaleY)
{
	//스케일 처리..
	int width = (int)(w * scaleX);
	int height = (int)(h * scaleY);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255 * alpha;		//혼합 비율.30%. 255=100% 불투명 (원본). 0=완전 투명★
	bf.AlphaFormat = AC_SRC_ALPHA;

	//투명색 처리.
	GdiAlphaBlend(hdc, x, y, width, height,
		pSpr->dc, sprX, sprY, width, height,  //<-- 필요하다면, 일정 부분 잘라서 그리기도 가능하겠지요.
		bf);
}

bool DrawAnimation(float dTime, int aniFPS, int x, int y, PANISPRITE pSpr, float alpha, float ScaleX, float ScaleY, COLORREF transColor)
{
	if (pSpr->aniTime > FPS(aniFPS))
	{
		pSpr->aniTime = 0;

		++pSpr->aniFrame;
		if (pSpr->aniFrame >= pSpr->cutX * pSpr->cutY)
		{
			pSpr->aniFrame = 0;

			pSpr->currLoopCount++;
		}
	}
	else
	{
		pSpr->aniTime += dTime;
	}

	int width = pSpr->spr.Width * ScaleX; // 한 컷의 너비
	int height = pSpr->spr.Height * ScaleY; // 한 컷의 높이

	//GdiTransparentBlt(backBufferDC, x, y, width, height, pSpr->spr.dc, (pSpr->aniFrame % pSpr->cutX) * width, 0, width, height, transColor);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255 * alpha;		//혼합 비율.30%. 255=100% 불투명 (원본). 0=완전 투명★
	bf.AlphaFormat = AC_SRC_ALPHA;

	GdiAlphaBlend(backBufferDC, x, y, width, height, pSpr->spr.dc, (pSpr->aniFrame % pSpr->cutX) * width, 0, width, height, bf);

	if (pSpr->loopCount == -1) return false;	//무한 재생
	else if (pSpr->currLoopCount == pSpr->loopCount)
	{
		pSpr->currLoopCount = 0;
		return true;
	}
	else
	{
		return false;
	}

}

void TextureDrawRot(HDC hdc, int x, int y, PSPRITE pSpr, float angle, float scaleX, float scaleY, COLORREF transColor, BOOL bDebug)
{
	//사용자 입력 스케일 처리..
	int width = (int)(pSpr->Width * scaleX);
	int height = (int)(pSpr->Height * scaleY);
	int& ow = width;
	int& oh = height;


	//(회전 비트멥) 저장용 리소스 : '메모리DC' 의 크기 결정 
	int rotSize = (width >= height) ? width : height;
	int rw = (int)((float)rotSize * 1.2f);		//조금더 키운다.20%
	int rh = (int)((float)rotSize * 1.2f);

	//(회전 비트멥) 결과 저장용 리소스 생성 : '쓰고 버리는 1회용' 속도 저하 요인. 스프라이트 별로 미리 적당한 '회전용' 버퍼를 만들어 쓰는 것은 어떨지?
	HDC hRotMemDC = CreateCompatibleDC(hdc);
	HBITMAP hRotBmp = CreateCompatibleBitmap(hdc, rw, rh);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(hRotMemDC, hRotBmp);

	//(회전 비트멥 ) 결과 저장 리소스 클리어.(+투명색)
	HBRUSH hbrBack = CreateSolidBrush(bDebug ? RGB(0, 255, 0) : transColor);		//지정 투명색으로 클리어.
	//HBRUSH hbrBack      = CreateSolidBrush( RGB(0, 255, 0) );         //디버그용 (녹색)
	//HBRUSH hbrBack      = CreateSolidBrush( transColor );				//지정 투명색으로 클리어.
	HBRUSH hbrOld = (HBRUSH)SelectObject(hRotMemDC, hbrBack);
	PatBlt(hRotMemDC, 0, 0, rw, rh, PATCOPY);
	DeleteObject(SelectObject(hRotMemDC, hbrOld));

	//(회전 비트멥) 회전 처리를 위해 목적DC 의 '좌표'계 변환.★
	::SetGraphicsMode(hRotMemDC, GM_ADVANCED);
	//::SetMapMode(hdc, MM_LOENGLISH); 

	// 라디안 구하기 : 미리 계산해서 속도향상을 유도한다.
	float rad = ToRadian(angle);
	float cs = cos(rad);
	float si = sin(rad);

	// 회전 행렬 (3x3) 구성 : 사용자 지정 "scale" 역시 적용.★
	// MATRIX = {  
	//    11  12  13    
	//    21  22  23
	//    31  32  33     
	//  };
	//
	// 단위행렬( Unit Matrix )/ = {  
	//    1  0  0		 
	//    0  1  0
	//    0  0  1		 
	// };
	XFORM xform;
	xform.eM11 = cs * scaleX;
	xform.eM12 = si * scaleX;
	xform.eM21 = -si * scaleY;
	xform.eM22 = cs * scaleY;
	xform.eDx = (float)rw / 2.0f;				//스프라이트 이동..(회전된 크기만큼)
	xform.eDy = (float)rh / 2.0f;				//적절히 조절하면, '괘도' 회전도 가능.

	// (회전 비트멥) 결과 저장용 '목적DC' 의 좌표계 '변환' ★
	::SetWorldTransform(hRotMemDC, &xform);

	// 스프라이트 출력 : 위에서 적용된 3x3 변환행렬(회전+스케일)이 적용된다.
	int dx = (int)((float)ow / (2 * scaleX));
	int dy = (int)((float)oh / (2 * scaleY));
	BitBlt(hRotMemDC, -dx, -dy, rw, rh, pSpr->dc, 0, 0, SRCCOPY);

	//몇가지 테스트용 코드들.
	//BitBlt(hRotMemDC, 0, 0, rw, rh, pSpr->hSurface, 0, 0, SRCCOPY );     
	//BitBlt(hRotMemDC, -(ow/2), -(oh/2), rw, rh, pSpr->hSurface, 0, 0, SRCCOPY );     
	//StretchBlt(hRotMemDC, -(ow/2), -(oh/2), rw, rh, pSpr->hSurface, 0, 0, rw, rh, SRCCOPY );     

	//---------------------------------------------------------------------------------------------
	// 회전 결과가 기록된 메모리DC 에서, '렌더타겟' 으로의 바로 출력은 불가능. : "좌표계"가 다르다!★
	// 따라서 '회전된 비트멥' 만을 따로 분리, "정상 : 정규 좌표계" 상태에서 '렌더타겟'으로 출력해야 한다.
	//---------------------------------------------------------------------------------------------
	// 목적DC 에서 회전 비트멥의 분리를 위해 '이전(비어있는..)' 비트멥을 재설정.  
	// : 이제 회전된 비트멥을 다른 'DC' 에서 사용할 수 있게 되었다.
	SelectObject(hRotMemDC, hbmOldDest);

	//회전된 비트멥을 '새 비어있는' 메모리 DC 에 붙인다.  : "렌더타겟"으로의 출력이 목적.★
	HDC hTempMemDC = CreateCompatibleDC(hdc);
	SelectObject(hTempMemDC, hRotBmp);


	//(백버퍼) 화면에 출력.★
	if (bDebug)
	{
		//이하 테스트 코드들... 
		BitBlt(hdc, x, y, rw, rh, hTempMemDC, 0, 0, SRCCOPY);
		//BitBlt( hdc, x-rw/2, y-rh/2, rw, rh, hTempMemDC, 0, 0, SRCCOPY); 
	}
	else
	{	//회전+반투명 처리..
		::GdiTransparentBlt(hdc, x, y, rw, rh, hTempMemDC, 0, 0, rw, rh, transColor);
	}

	// 그래픽 모드 복구 : 회전처리용 메모리DC 는 제거될 것이라면, '복구' 는 무의미.
	//SetGraphicsMode(hRotMemDC, GM_COMPATIBLE);    

	//리소스 해제.
	DeleteDC(hRotMemDC);
	DeleteDC(hTempMemDC);
	DeleteObject(hRotBmp);
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF col, int width)
{
	HPEN pen = CreatePen(PS_SOLID, width, col);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, oldpen);
	DeleteObject(pen);
}

void DrawFillRect(HDC hdc, int x, int y, int width, int height, COLORREF fillCol, COLORREF lineCol, int lineWidth)
{
	HPEN pen = CreatePen(PS_SOLID, lineWidth, lineCol);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);

	HBRUSH hBrush = CreateSolidBrush(fillCol);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, x, y, x + width, y + height);

	SelectObject(hdc, oldpen);
	SelectObject(hdc, oldBrush);

	DeleteObject(pen);
	DeleteObject(hBrush);
}

void DrawHollowRect(HDC hdc, int x, int y, int width, int height, COLORREF lineCol, int lineWidth)
{
	HPEN pen = CreatePen(PS_SOLID, lineWidth, lineCol);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(HOLLOW_BRUSH));

	Rectangle(hdc, x, y, x + width, y + height);

	SelectObject(hdc, oldpen);
	SelectObject(hdc, oldBrush);

	DeleteObject(pen);
}