#include <Windows.h>
#include "SysMetric.h"

TCHAR szClassName[] = L"szMyClass";



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cyChar, cxUpperChar, cyClient, cxClient,cxMaxWidth;
	int iVertPos,iHorzPos,i,iPaintBeg,iPaintEnd,X,Y;
	HDC hdc;
	TEXTMETRIC tm;
	SCROLLINFO si;
	PAINTSTRUCT ps;
	TCHAR szBuff[10];

	switch(msg)
	{
	case WM_CREATE:
		{
			hdc = GetDC(hwnd);
			
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			cxUpperChar = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
			cxMaxWidth = 22 * cxUpperChar + 40 * cxChar;
			ReleaseDC(hwnd, hdc);
		}
		break;
	case WM_SIZE:
		{
			cyClient = HIWORD(lParam);
			cxClient = LOWORD(lParam);

			// set vertical scroll bar range adn page.
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMax = numlines - 1;
			si.nMin = 0;
			si.nPage = cyClient / cyChar;
			/*
			1.Set rang  to si.nMin and si.nMax.
			2.The SetScrollInfo function performs range checking on the values specified by the nPage and nPos members 
			of the SCROLLINFO structure. 
			3.The nPage member must specify a value from 0 to nMax - nMin +1.
			A scroll bar uses this value to determine the appropriate size of the proportional scroll box.
			4.The nPos member must specify a value between nMin and nMax - max( nPage– 1, 0). 
			If either value is beyond its range, the function sets it to a value that is just within the range.
			We don't give the nPos value, system help us initial the value.
			*/
			SetScrollInfo(hwnd, SB_VERT, &si, true);

			//set horizontal scroll bar range adn page.
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMax = 2 + cxMaxWidth/cxChar;
			si.nMin = 0;
			si.nPage = cxClient / cxChar;
			SetScrollInfo(hwnd, SB_HORZ, &si, true);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);

			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			GetScrollInfo(hwnd, SB_VERT, &si);
			iVertPos = si.nPos;

			GetScrollInfo(hwnd, SB_HORZ, &si);
			iHorzPos = si.nPos;
			
			/*
			1. PAINTSTRUCT rcPaint A RECT structure that specifies the upper left and lower right corners 
			of the rectangle in which the painting is requested, in device units relative to the upper-left corner of the client area.
			2.RECT.top: The y-coordinate of the upper-left corner of the rectangle.
			*/
			iPaintBeg = max (0, iVertPos + ps.rcPaint.top/cyChar);
			iPaintEnd = min (numlines - 1, iVertPos + ps.rcPaint.bottom / cyChar);

			for(i = iPaintBeg; i <= iPaintEnd; i++)
			{
				Y = cyChar * (i - iVertPos);
				X = cxChar * (1 - iHorzPos);
				TextOut(hdc, X, Y, sysMetric[i].szLabel, lstrlen(sysMetric[i].szLabel));
				TextOut(hdc, X + 22*cxUpperChar, Y, sysMetric[i].szDesc, lstrlen(sysMetric[i].szDesc));

				SetTextAlign(hdc, TA_LEFT | TA_TOP);

				TextOut(hdc, X + 22*cxUpperChar + 40*cxChar, Y, szBuff, wsprintf(szBuff, L"%5d", GetSystemMetrics(sysMetric[i].iIndex)));

				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}

			EndPaint(hwnd, &ps);
		}
		break;
	case WM_VSCROLL:
		{
			si.cbSize = sizeof(si);
			//Combination of SIF_PAGE, SIF_POS, SIF_RANGE, and SIF_TRACKPOS.
			si.fMask = SIF_ALL;
			//The GetScrollInfo function enables applications to use 32-bit scroll positions. 
			//Although the messages that indicate scroll bar position, WM_HSCROLL and WM_VSCROLL, provide only 16 bits of position data
			GetScrollInfo(hwnd, SB_VERT, &si);


			iVertPos = si.nPos;
			switch(LOWORD(wParam))
			{
			case SB_TOP:
				si.nPos = si.nMin;
				break;
			case SB_BOTTOM:
				si.nPos = si.nMax;
				break;
			case SB_LINEUP:
				si.nPos -= 1;
				break;
			case SB_LINEDOWN:
				si.nPos += 1;
				break;
			case SB_PAGEUP:
				si.nPos -= si.nPage;
				break;
			case SB_PAGEDOWN:
				si.nPos += si.nPage;
				break ;
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			default:
				break;
			}

			si.fMask = SIF_POS;
			SetScrollInfo (hwnd, SB_VERT, &si, TRUE);
			// after WM_VSCROLL get new si.nPos
			GetScrollInfo (hwnd, SB_VERT, &si);

			if(iVertPos != si.nPos)
			{
				/*
				1.(3rd parameter)This parameter must be a negative value to scroll the content of the window up.
				2.The area uncovered by ScrollWindow is not repainted, but it is combined into the window's update region. 
				The application eventually receives a WM_PAINT message notifying it that the region must be repainted. 
				To repaint the uncovered area at the same time the scrolling is in action, 
				call the UpdateWindow function immediately after calling ScrollWindow.
				*/
				ScrollWindow (hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL) ;
				UpdateWindow (hwnd) ;
			}
		}
		break;
	case WM_HSCROLL:
		{
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hwnd, SB_HORZ, &si);

			iHorzPos = si.nPos;
			switch(LOWORD(wParam))
			{
			case SB_LEFT:
				si.nPos = si.nMin;
				break;
			case SB_RIGHT:
				si.nPos = si.nMax;
				break;
			case SB_LINELEFT:
				si.nPos -= 1;
				break;
			case SB_LINERIGHT:
				si.nPos += 1;
				break;
			case SB_PAGELEFT:
				si.nPos -= si.nPage;
				break;
			case SB_PAGERIGHT:
				si.nPos += si.nPage;
				break;
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			default:
				break;
			}

			si.fMask = SIF_POS;
			SetScrollInfo(hwnd, SB_HORZ, &si, true);
			GetScrollInfo(hwnd, SB_HORZ, &si);

			if(iHorzPos != si.nPos)
			{
				ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
				UpdateWindow(hwnd);
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
	
	hWnd = CreateWindow(szClassName, L"HelloWorld", WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, 650, 400, NULL, NULL, hInstance, NULL);

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