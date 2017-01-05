#include <Windows.h>
#include <algorithm>
#include "SysMetric2.h"


TCHAR szClassName[] = L"szMyClass";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	TCHAR szBuff[10];

	static LONG cxWidth, cyHeigh, cxUpperCase;
	static int cyClient, iVscrollPos = 0;
	int i,Y,lines;

	switch(msg)
	{
	case WM_CREATE:
		{
			hdc = GetDC(hwnd);
			GetTextMetrics(hdc, &tm);
			ReleaseDC(hwnd, hdc);
			cxWidth = tm.tmAveCharWidth;
			cyHeigh = tm.tmHeight + tm.tmExternalLeading;
			cxUpperCase = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxWidth / 2;	

			SetScrollRange(hwnd, SB_VERT, 0, numlines - 1, false);
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, true);
		}
		break;

	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			for(i=0; i<numlines; i++)
			{
				Y = cyHeigh*(i - iVscrollPos);
				TextOut(hdc, 0, Y, sysMetric[i].szLabel, lstrlen(sysMetric[i].szLabel));
				TextOut(hdc, 22* cxUpperCase, Y, sysMetric[i].szDesc, lstrlen(sysMetric[i].szDesc));

				SetTextAlign(hdc, TA_LEFT | TA_TOP);

				TextOut(hdc, 22* cxUpperCase + 40*cxWidth, Y, szBuff, wsprintf(szBuff, L"%5d", GetSystemMetrics(sysMetric[i].iIndex)));
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_SIZE:
		cyClient = HIWORD(lParam); // The high-order word of lParam specifies the new height of the client area.
		break;
	case WM_VSCROLL:
		{
			switch (LOWORD (wParam))
			{
				case SB_LINEUP:
					iVscrollPos -= 1 ;
				break ;
				case SB_LINEDOWN:
					iVscrollPos += 1 ;
				break ;
				case SB_PAGEUP:
					iVscrollPos -= cyClient / cyHeigh ;
				break ;
				case SB_PAGEDOWN:
					iVscrollPos += cyClient / cyHeigh ;
				break ;
				//The HIWORD specifies the current position of the scroll box if the LOWORD is SB_THUMBPOSITION or SB_THUMBTRACK; otherwise, this word is not used.
				case SB_THUMBTRACK:
					iVscrollPos = HIWORD (wParam) ;
				break ;
				default:
				break ;
			}

			lines = numlines - 1;
			iVscrollPos = max (0, min (iVscrollPos, lines)) ;
		
			if (iVscrollPos != GetScrollPos (hwnd, SB_VERT))
			{
				SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE) ;
				// redraw the hole client area.
				InvalidateRect (hwnd, NULL, TRUE) ;
			}
		}
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
	
	hWnd = CreateWindow(szClassName, L"HelloWorld", WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, 650, 400, NULL, NULL, hInstance, NULL);

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