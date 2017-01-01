#include <Windows.h>

TCHAR szClassName[] = L"szMyClass";

/*
How to create a window ?
1.CreateWindow return a hanle to window.
2.CreateWindow function need a registered WNDCLASS.
3.WNDCLASS need a WndProc.
4.After window created, ShowWindow() and UpdateWindow().
5.Now the window is "static", you nedd add message loop to make it "dynamic".
*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	//The PAINTSTRUCT structure contains information for an application. 
	//This information can be used to paint the client area of a window owned by that application.
	PAINTSTRUCT ps;
	//The RECT structure defines the coordinates of the upper-left and lower-right corners of a rectangle.
	RECT rect;

	switch(msg)
	{
	case WM_CREATE:
		PlaySound (L"hello.wav", NULL, SND_FILENAME | SND_ASYNC) ;
		break;
	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps);
		GetClientRect (hwnd, &rect) ;
		DrawText (hdc, TEXT ("Hello, Windows 98!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
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

/*******
What is message  loop?
1.The message loop calls GetMessage(), which looks in your message queue. 
If the message queue is empty your program basically stops and waits for one (it Blocks).
2.When an event occures causing a message to be added to the queue (for example the system registers a mouse click) 
GetMessages() returns a positive value indicating there is a message to be processed, 
and that it has filled in the members of the MSG structure we passed it. It returns 0 if it hits WM_QUIT, 
and a negative value if an error occured.
3.We take the message (in the Msg variable) and pass it to TranslateMessage(), this does a bit of additional processing, 
translating virtual key messages into character messages. This step is actually optional, 
but certain things won't work if it's not there.
4.Once that's done we pass the message to DispatchMessage(). What DispatchMessage() does is take the message, 
checks which window it is for and then looks up the Window Procedure for the window. It then calls that procedure, 
sending as parameters the handle of the window, the message, and wParam and lParam.
5.In your window procedure you check the message and it's parameters, 
and do whatever you want with them! If you aren't handling the specific message, 
you almost always call DefWindowProc() which will perform the default actions for you (which often means it does nothing).
6.Once you have finished processing the message, your windows procedure returns, DispatchMessage() returns, 
and we go back to the beginning of the loop.
*******/




