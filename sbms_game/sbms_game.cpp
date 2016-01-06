#include <windows.h>
#include <gdiplus.h>
#include <time.h>
#include "resource.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus")



HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

int screen_mode,choice;
clock_t start_anim_time;
enum screen {title, choose, tutorial, start_anim, ingame, gameover};

void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND     hWnd;
	MSG		 msg;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok) 
	{
		MessageBox(NULL, TEXT("GDI+ 라이브러리를 초기화할 수 없습니다."),TEXT("알림"), MB_OK);
		return 0;
	}


	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"Window Class Name";
	RegisterClass(&WndClass);
	hWnd = CreateWindow(
		L"Window Class Name",
		L"Window Title Name",
		WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXFULLSCREEN)/2-408,
		GetSystemMetrics(SM_CYFULLSCREEN)/2-319,
		816,
		638,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	
	HBITMAP hBit, OldBit;
	RECT crt;

	int start0_anim[5] = { START0_ANI0,START0_ANI1,START0_ANI2,START0_ANI3,START0_ANI4 };
	int start0_word[4] = { START0_WORD0, START0_WORD1, START0_WORD2, START0_WORD3 };
	int start0_anim_time[6] = { 0,350,450,550,650,750 };

	int start1_anim[6] = { START1_ANI0,START1_ANI1,START1_ANI2,START1_ANI3,START1_ANI4,START1_ANI5 };
	int start1_word[4] = { START1_WORD0, START1_WORD1, START1_WORD2, START1_WORD3 };
	int start1_anim_time[7] = { 0, 90, 180, 360, 400, 580, 800 };


	switch (iMsg)
	{
	case WM_CREATE:
		screen_mode = screen::title;
		choice = 0;
		SetTimer(hWnd, 1, 10, 0);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);

		MemDC = CreateCompatibleDC(hdc);
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBit = (HBITMAP)SelectObject(MemDC, hBit);	
		SetBkColor(MemDC, RGB(255, 255, 255));

		switch (screen_mode)
		{
		case screen::title:
			if (choice==0)
				OnPaint(MemDC, TITLE0, 0, 0);
			else
				OnPaint(MemDC, TITLE1, 0, 0);
			break;
		case screen::tutorial:
			OnPaintA(MemDC, TITLE1, 0, 0, 0.7);
			break;
		case screen::choose:
			if(choice==0)
				OnPaint(MemDC, SELECT_KUDA, 0, 0);
			else
				OnPaint(MemDC, SELECT_NOBO, 0, 0);
			break;
		case screen::start_anim:
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
				{
					if (750*i+start0_anim_time[j] <= clock()-start_anim_time && clock() - start_anim_time <= 750 * i + start0_anim_time[j+1])
					{
						OnPaint(MemDC, start0_anim[j], 0, 0);
						if (j < 2 && i != 0)
						{
							OnPaint(MemDC, start0_word[i - 1], 0, 0);
						}
						else if (j >= 2)
						{
							OnPaint(MemDC, start0_word[i], 0, 0);
						}
					}
				}

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 7; j++)
				{
					if (800 * i + start1_anim_time[j] <= clock() - start_anim_time-3000 && clock() - start_anim_time-3000 <= 800 * i + start1_anim_time[j + 1])
					{
						OnPaint(MemDC, start1_anim[j], 0, 0);
						if (j < 2 && i != 0)
						{
							OnPaint(MemDC, start1_word[i - 1], 0, 0);
						}
						else if (j >= 2)
						{
							OnPaint(MemDC, start1_word[i], 0, 0);
						}
					}
				}
			break;

		}


		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		DeleteObject(hBit);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			switch (screen_mode)
			{
			case screen::title:
				if (choice == 1)
					choice = 0;
				break;
			}
			break;
		case VK_DOWN:
			switch (screen_mode)
			{
			case screen::title:
				if (choice == 0)
					choice = 1;
				break;
			}
			break;
		case VK_LEFT:
			switch (screen_mode)
			{
			case screen::choose:
				if (choice == 1)
					choice = 0;
				break;
			}
			break;
		case VK_RIGHT:
			switch (screen_mode)
			{
			case screen::choose:
				if (choice == 0)
					choice = 1;
				break;
			}
			break;
		case VK_RETURN:
			switch (screen_mode)
			{
			case screen::title:
				if (choice == 0)
					screen_mode = screen::choose;
				else
					screen_mode = screen::tutorial;
				break;
			case screen::choose:
				start_anim_time = clock();
				screen_mode = screen::start_anim;
				break;
			}
			break;
		case 'Z':
			switch (screen_mode)
			{
			case screen::title:
				if (choice == 0)
					screen_mode = screen::choose;
				else
					screen_mode = screen::tutorial;
				break;
			case screen::choose:
				start_anim_time = clock();
				screen_mode = screen::start_anim;
				break;
			}
			break;
		case 'X':
			switch (screen_mode)
			{
			case screen::tutorial:
				choice = 1;
				screen_mode = screen::title;
				break;
			case screen::choose:
				choice = 0;
				screen_mode = screen::title;
				break;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void OnPaint(HDC hdc, int ID, int x, int y)
{
	Graphics G(hdc);
	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(ID), TEXT("PNG"));
	if (!hResource)
		return;

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image I(pStream);
	pStream->Release();
	if (I.GetLastStatus() != Ok) return;

	G.DrawImage(&I, x, y, I.GetWidth(), I.GetHeight());
}


void OnPaintA(HDC hdc, int ID, int x, int y, double alpha)
{
	Graphics G(hdc);
	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(ID), TEXT("PNG"));
	if (!hResource)
		return;

	ColorMatrix ClrMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ImgAttr;
	ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image I(pStream);
	pStream->Release();
	if (I.GetLastStatus() != Ok) return;

	RectF destination(0, 0, I.GetWidth(), I.GetHeight());
	G.DrawImage(&I, destination, x, y, I.GetWidth(), I.GetHeight(), UnitPixel, &ImgAttr);
}