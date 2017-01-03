#include <Windows.h>
#include "SysMetric.h"

TCHAR szClassName[] = L"szMyClass";


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	TEXTMETRIC tm;
	PAINTSTRUCT ps;
	LONG characterWidth, characterHeight, characterApp;
	TCHAR szBuffer[10];

	/*
	Two ways to get HDC.
	one:
	GetDC(hwnd);
	ReleaseDC(hwnd, hdc);
	two:
	BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
	The difference is GetDC function don't validate the invalidate rectangle.
	So the device context handle return from GetDC function is used to deal with Keyboard or Mouse message!
	*/

	hdc = GetDC(hwnd);
	/*
	TEXTMETRIC structure contain sysytem from information.
	*/
	GetTextMetrics(hdc, &tm);
	//The average width of characters in the font
	characterWidth = tm.tmAveCharWidth;// 7
	//tmHeight: The height (ascent + descent) of characters.
	//tmExternalLeading: The amount of extra leading (space) that the application adds between rows.
	characterHeight = tm.tmHeight + tm.tmExternalLeading;//16
	//tmPitchAndFamily: Specifies information about the pitch, the technology, and the family of a physical font.
	characterApp = (tm.tmPitchAndFamily & 1 ? 3 : 2) * characterWidth / 2; //10
	ReleaseDC(hwnd, hdc);

	switch(msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		for(int i=0; i< numlines; i++)
		{
			// the textout function display a string line on video window.
			TextOut(hdc, 0, characterHeight*i, sysMetric[i].szLabel, wcslen(sysMetric[i].szLabel));
			TextOut(hdc, 22*characterApp, characterHeight*i, sysMetric[i].szDesc, wcslen(sysMetric[i].szDesc));
			SetTextAlign(hdc, TA_LEFT | TA_TOP);

			TextOut(hdc, 22*characterWidth + 40*characterApp, characterHeight*i, szBuffer, wsprintf(szBuffer, L"%5d", GetSystemMetrics(sysMetric[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}	
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClass(&wc))
	{
		MessageBox(NULL, L"can't register windowclass", L"Error", MB_OK | MB_ICONINFORMATION);
		return 0;
	}
	
	hWnd = CreateWindow(szClassName, L"HelloWorld", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 650, 400, NULL, NULL, hInstance, NULL);

	if(hWnd == NULL)
	{
		MessageBox(NULL, L"can't create window", L"Error", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}