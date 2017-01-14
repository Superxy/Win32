#include "DeviceContext1.h"
#include <Windows.h>

TCHAR szWndClassName[] = L"DeviceContext";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cxUpperChar, cyChar;
	TEXTMETRIC tm;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuff[10];

	switch(uMsg)
	{
	case WM_CREATE:
		{
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			cxUpperChar = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
			ReleaseDC(hwnd, hdc);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			for(int i=0; i<NUMLINES; i++)
			{
				TextOut(hdc, 0, cyChar*i, devcaps[i].szLabel, lstrlen(devcaps[i].szLabel));
				TextOut(hdc, 12*cxUpperChar, cyChar*i, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));

				SetTextAlign(hdc, TA_LEFT | TA_TOP);
				
				TextOut(hdc, 12*cxUpperChar+28*cxChar, cyChar*i, szBuff, wsprintf(szBuff, L"%5d", GetDeviceCaps(hdc, devcaps[i].iIndex)));
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	HWND hwnd;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = szWndClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClass(&wc))
	{
		MessageBox(NULL, L"Can't register class", L"Error", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	hwnd = CreateWindow(szWndClassName, L"Device", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 450, 400, NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, L"Can't create window", L"Error", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}