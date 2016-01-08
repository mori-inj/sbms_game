#include <Windows.h>
#include <gdiplus.h>
#include <time.h>
#include <tchar.h>
#include "resource.h"

using namespace std;
using namespace Gdiplus;
#pragma comment(lib, "gdiplus")



HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

int screen_mode,choice,turn,player_input,score,nth=1,before_score,before_nth,is_start=0,time_limit=800;
clock_t start_anim_time;
enum screen {title, choose, tutorial, start_anim, ingame, gameover};

void OnFont(HDC hdc, WCHAR* input, int size);
void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
int logi2(int n);
int isCorrect(int input, int score, int nth);
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
	WndClass.lpszClassName = L"SBMS_GAME";
	RegisterClass(&WndClass);
	hWnd = CreateWindow(
		L"SBMS_GAME",
		L"SBMS_GAME",
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

	WCHAR score_t[256];
	HBITMAP hBit, OldBit;
	RECT crt;
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = 100;
	rect.bottom = 50;

	int start0_anim[5] = { START0_ANI0,START0_ANI1,START0_ANI2,START0_ANI3,START0_ANI4 };
	int start0_word[4] = { START0_WORD0, START0_WORD1, START0_WORD2, START0_WORD3 };
	int start0_anim_time[6] = { 0,350,450,550,650,750 };

	int start1_anim[6] = { START1_ANI0,START1_ANI1,START1_ANI2,START1_ANI3,START1_ANI4,START1_ANI5 };
	int start1_word[4] = { START1_WORD0, START1_WORD1, START1_WORD2, START1_WORD3 };
	int start1_anim_time[7] = { 0, 90, 180, 360, 400, 580, 800 };

	int kud[4] = { KUD0, KUD1, KUD2, KUD3 };
	int ingame_time[4] = { 0,50,150,200 };
	int nob[4] = { NOB0, NOB1, NOB2, NOB3 };


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
				for (int j = 0; j < 6; j++)
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

			if (clock() - start_anim_time >= 6200)
			{
				screen_mode = screen::ingame;
				start_anim_time = clock();
				turn = -1;
			}
			break;

		case screen::ingame:
			if (turn == 0)
			{
				if (clock()-start_anim_time >= time_limit)
				{
					screen_mode = screen::gameover;
				}
				else if (clock()-start_anim_time>=200)
				{
					OnPaint(MemDC, EMPTY, 0, 0);
					swprintf_s(score_t, L"%d",score);
					OnFont(MemDC, score_t, 64);
					OnPaint(MemDC, kud[0], 0, 0);
					OnPaint(MemDC, nob[0], 0, 0);
				}
				else if(0<=clock()-start_anim_time && clock()-start_anim_time <=200)
				{
					for (int i = 0; i < 3; i++)
					{
						if (ingame_time[i] <= clock() - start_anim_time && clock() - start_anim_time <= ingame_time[i + 1])
						{
							OnPaint(MemDC, EMPTY, 0, 0);
							swprintf_s(score_t, L"%d", score);
							OnFont(MemDC, score_t, 64);
							if (choice == 1)
							{
								OnPaint(MemDC, nob[0], 0, 0);
								OnPaint(MemDC, kud[i + 1], 0, 0);
								if (isCorrect(0, before_score, before_nth))
									OnPaint(MemDC, ZERO_K, 0, 0);
								else
									OnPaint(MemDC, ONE_K, 0, 0);
							}
							else
							{
								OnPaint(MemDC, kud[0], 0, 0);
								OnPaint(MemDC, nob[i + 1], 0, 0);
								if (isCorrect(0, before_score, before_nth))
									OnPaint(MemDC, ZERO_N, 0, 0);
								else
									OnPaint(MemDC, ONE_N, 0, 0);
							}
						}
					}
				}
			}
			else
			{
				if (turn!=-1 && 0 <= clock() - start_anim_time && clock() - start_anim_time <= 200)
				{
					for (int i = 0; i < 3; i++)
					{
						if (ingame_time[i] <= clock() - start_anim_time && clock() - start_anim_time <= ingame_time[i + 1])
						{
							OnPaint(MemDC, EMPTY, 0, 0);
							swprintf_s(score_t, L"%d", score);
							OnFont(MemDC, score_t, 64);
							if (choice == 0)
							{
								OnPaint(MemDC, nob[0], 0, 0);
								OnPaint(MemDC, kud[i + 1], 0, 0);
								if (player_input==0)
									OnPaint(MemDC, ZERO_K, 0, 0);
								else
									OnPaint(MemDC, ONE_K, 0, 0);
							}
							else
							{
								OnPaint(MemDC, kud[0], 0, 0);
								OnPaint(MemDC, nob[i + 1], 0, 0);
								if (player_input == 0)
									OnPaint(MemDC, ZERO_N, 0, 0);
								else
									OnPaint(MemDC, ONE_N, 0, 0);
							}
						}
					}
				}
				else if(clock() - start_anim_time >= time_limit)
				{
					OnPaint(MemDC, EMPTY, 0, 0);
					swprintf_s(score_t, L"%d", score);
					OnFont(MemDC, score_t, 64);
					OnPaint(MemDC, kud[0], 0, 0);
					OnPaint(MemDC, nob[0], 0, 0);

					start_anim_time = clock();
					turn = 0;

					before_nth = nth;
					before_score = score;

					nth++;
					if (nth>1 + logi2(score))
					{
						score++;
						nth = 1;
					}

					if (score % 8 == 0 && time_limit > 400)
						time_limit -= 30;
				}
				else //if (200 <= clock() - start_anim_time && clock() - start_anim_time <= time_limit)
				{
					OnPaint(MemDC, EMPTY, 0, 0);
					swprintf_s(score_t, L"%d", score);
					OnFont(MemDC, score_t, 64);
					OnPaint(MemDC, kud[0], 0, 0);
					OnPaint(MemDC, nob[0], 0, 0);
				}
			}
			break;
		case screen::gameover:
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
			case screen::gameover:
				choice = 0;
				screen_mode = title;
				nth = before_nth = 1;
				score = before_score = 0;
				is_start = 0;
				time_limit = 800;

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
			case screen::gameover:
				choice = 0;
				screen_mode = title;
				nth = before_nth = 1;
				score = before_score = 0;
				is_start = 0;
				time_limit = 800;
				break;
			case screen::ingame:
				if (turn == 0 )
				{
					player_input = 0;
					start_anim_time = clock();
					turn = 1;

					before_nth = nth;
					before_score = score;

					nth++;
					if (nth>1 + logi2(score))
					{
						score++;
						nth = 1;
					}

					if (score % 8 == 0 && time_limit > 400)
						time_limit -= 30;

					if (isCorrect(0, before_score, before_nth) == 0)
						screen_mode = screen::gameover;
				}
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
			case screen::gameover:
				choice = 0;
				screen_mode = title;
				nth = before_nth = 1;
				score = before_score = 0;
				is_start = 0;
				time_limit = 800;
				break;
			case screen::ingame:
				if (turn == 0 )
				{
					player_input = 1;
					start_anim_time = clock();
					turn = 1;

					before_nth = nth;
					before_score = score;

					nth++;
					if (nth>1 + logi2(score))
					{
						score++;
						nth = 1;
					}

					if (score % 8 == 0 && time_limit > 400 && turn != 1)
						time_limit -= 30;

					if (isCorrect(1, before_score, before_nth) == 0)
						screen_mode = screen::gameover;
				}
				break;
			}
			break;
		case '0':
			switch (screen_mode)
			{
			case screen::ingame:
				if (turn == 0 )
				{
					player_input = 0;
					start_anim_time = clock();
					turn = 1;

					before_nth = nth;
					before_score = score;

					nth++;
					if (nth>1 + logi2(score))
					{
						score++;
						nth = 1;
					}

					if (score % 8 == 0 && time_limit > 400 && turn != 1)
						time_limit -= 30;

					if (isCorrect(0, before_score, before_nth) == 0)
						screen_mode = screen::gameover;
				}
				break;
			}
			break;
		case '1':
			switch (screen_mode)
			{
			case screen::ingame:
				if (turn == 0 )
				{
					player_input = 1;
					start_anim_time = clock();
					turn = 1;

					before_nth = nth;
					before_score = score;

					nth++;
					if (nth>1 + logi2(score))
					{
						score++;
						nth = 1;
					}

					if (score % 8 == 0 && time_limit > 400)
						time_limit -= 30;

					if (isCorrect(1, before_score, before_nth) == 0)
						screen_mode = screen::gameover;
				}
				break;
			}
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void OnFont(HDC hdc, WCHAR* input, int size)
{
	Graphics G(hdc);
	
	Font F1(L"맑은 고딕", size, FontStyleRegular, UnitPixel);
	Font F2(L"서울한강체 M", size, FontStyleRegular, UnitPixel);
	
	RectF R(408, 0, 400, size);
	SolidBrush B(Color(0, 0, 0));
	Pen P(Color(0, 0, 0));
	StringFormat SF(StringFormatFlagsDirectionRightToLeft);

	SF.SetAlignment(StringAlignmentNear);
	SF.SetLineAlignment(StringAlignmentNear);
	//G.DrawRectangle(&P, R);
	FontFamily FM(L"서울한강체 M");
	if (!FM.IsAvailable())
		G.DrawString(input, -1, &F1, R, &SF, &B);
	else
		G.DrawString(input, -1, &F2, R, &SF, &B);
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


int logi2(int n)
{
	int cnt = 0;
	while (n>0)
	{
		n /= 2;
		cnt++;
	}
	return cnt == 0 ? 0 : cnt - 1;
}


int isCorrect(int input, int score, int nth)
{
	int i;
	int num = logi2(score) - nth + 1;
	for (i = 0; i<num; i++)
		score /= 2;
	if (input == score % 2)
		return 1;
	else
		return 0;
}