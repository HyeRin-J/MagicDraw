// MagicDraw.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "../RRYEngine/framework.h"
#include "../RRYEngine/Vector.h"
#include "Unit.h"
#include "Hero.h"
#include "UnitManager.h"
#include "System.h"
#include "vector"
#include "MagicManager.h"
#include "GameProc.h"
#include "SoundManager.h"
#include "../RRYEngine/Vector.h"

#include "resource.h"

#define MAX_LOADSTRING 100

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
char* szTitle = "Magic Draw";           // 제목 표시줄 텍스트입니다.
char* szWindowClass = "Magic Draw";        // 기본 창 클래스 이름입니다.

HWND g_hWnd;

void (*GameProcFunc[(int)Stage::STAGE_COUNT])()
= {
	Title,
	Loading,
	Tutorial,
	NormalStage,
	StageClear,
	IntroBossStage,
	BossStage,
	GameOver,
};

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.

	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			GameProcFunc[(int)StageInfo]();
		}
	}

	Release();
	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	return RegisterClassEx(&wcex);
}

void ResizeWindow(HWND hWnd, UINT NewWidth, UINT NewHeight)
{
	//현재 윈도우의 스타일 구하기.
	RECT oldrc;
	DWORD sytle = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);

	//현재 윈도우 '전체' 크기 (스크린좌표) 를 얻는다.
	::GetWindowRect(hWnd, &oldrc);

	//새로 생성될 윈도우의 '클라이언트' 영역 크기 계산하기.
	RECT newrc;
	newrc.left = 0;
	newrc.top = 0;
	newrc.right = NewWidth;
	newrc.bottom = NewHeight;

	//newrc 만큼의 클라이언트 영역을 포함하는 윈도우 '전체' 크기를 구한다.
	//현재 '메뉴' 는 없다는 가정하에 처리되고 있음.
	//계산된 결과는 newrc 에 다시 들어온다. ( 스크린 좌표 )
	//exstyle &= ~WS_EX_TOPMOST;
	::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);


	//보정된 윈도우의 너비와 폭을 구한다. 
	int width = (newrc.right - newrc.left);
	int height = (newrc.bottom - newrc.top);


	//새로운 크기를 윈도우에 설정한다.
	::SetWindowPos(hWnd, HWND_NOTOPMOST,
		oldrc.left, oldrc.top,
		width, height, SWP_SHOWWINDOW);

}

HANDLE buttonImage = nullptr;

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
	int width = System::GetInstance()->m_ScreenSize.cx;
	int height = System::GetInstance()->m_ScreenSize.cy;

	//WS_CLIPCHILDREN
	//부모 창 내에서 갱신이 발생할 때 자식 창이 차지하는 영역을 제외합니다.

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX) | WS_CLIPCHILDREN,
		100, 100, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	g_hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ResizeWindow(hWnd, width, height);


	g_ReplayBtn = CreateWindow(TEXT("button"), TEXT("REPLAY"), WS_CHILD | WS_VISIBLE | BS_BITMAP,
		583, 578, 273, 113, hWnd, (HMENU)0, hInst, NULL);
	buttonImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_REPLAYBTN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	SendMessage(g_ReplayBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)buttonImage);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

SPRITE spr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 0:
			StageInfo = Stage::title;
			g_Score = 0;
			WaveCount = 0;
			SoundManager::GetInstance()->PlayBackGround();
			System::GetInstance()->m_State = 0;
			break;
		}
	}
	break;

	case WM_CREATE:
	{
		//엔진 시작...
		CreateEngine();
		//데이터 로딩
		ynMMedia_Initial(hWnd);
		DataLoading();

		SoundManager::GetInstance()->PlayBackGround();

		//도형 그릴 백버퍼 생성
		HDC hdc = GetDC(hWnd);

		System::GetInstance()->m_ShapeBufferDC = CreateCompatibleDC(hdc);
		System::GetInstance()->m_HShapeBufferBmp = CreateCompatibleBitmap(hdc, System::GetInstance()->m_ScreenSize.cx, System::GetInstance()->m_ScreenSize.cy);
		SelectObject(System::GetInstance()->m_ShapeBufferDC, System::GetInstance()->m_HShapeBufferBmp);

		//화면 지우기
		DrawFillRect(System::GetInstance()->m_ShapeBufferDC, 0, 0,
			System::GetInstance()->m_ScreenSize.cx, System::GetInstance()->m_ScreenSize.cy,
			RGB(0, 0, 0));

		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		SetCapture(hWnd);

		if (StageInfo == Stage::title)
		{
			StageInfo = Stage::loading;
		}
		if (StageInfo == Stage::tutorial) StageInfo = Stage::stage;

		MagicManager::GetInstance()->m_DrawPoints.clear();

		LONG x = LOWORD(lParam);
		LONG y = HIWORD(lParam);

		MoveToEx(System::GetInstance()->m_ShapeBufferDC, x, y, NULL);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (wParam & VK_LBUTTON)
		{
			static int count = 0;

			static ULONGLONG startTime = GetTickCount64();
			ULONGLONG moveTime = GetTickCount64();

			LONG x = LOWORD(lParam);
			LONG y = HIWORD(lParam);

			//움직인 횟수 보정, 시간 보정
			++count %= 4;

			DrawAlphaSprite(System::GetInstance()->m_ShapeBufferDC, x - 10, y - 10, 20, 20,
				&MagicManager::GetInstance()->m_MagicStarEffect, 20 * (rand() % 5), 0);

			if (moveTime - startTime > 0.25f && count == 0)
			{
				MagicManager::GetInstance()->m_DrawPoints.push_back({ x, y });

				startTime = GetTickCount64();

				//체크 지점에 이펙트용 스프라이트 출력(현재 별)
			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		ReleaseCapture();

		//도형 체크하기
		if (!(UnitManager::GetInstance()->m_Hero)->m_IsFinish)
			MagicManager::GetInstance()->CheckShape();

		//화면 지우기
		DrawFillRect(System::GetInstance()->m_ShapeBufferDC, 0, 0,
			System::GetInstance()->m_ScreenSize.cx, System::GetInstance()->m_ScreenSize.cy,
			RGB(0, 0, 0));

		MagicManager::GetInstance()->m_DrawPoints.clear();
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
