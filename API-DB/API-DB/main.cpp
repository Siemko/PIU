#include <Windows.h>
#include <WindowsX.h>
#include <vector>
#include <string>

TCHAR ClassName [] = L"Nazwa Klasy";
RECT size;
int x;
HINSTANCE global;
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
			x=10;
			GetClientRect(hwnd, &size);
			SetTimer(hwnd, 832, 4000, NULL);
		}break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 832: {InvalidateRect(hwnd, NULL, true); x+=rand()%10+2;}break;
			}break;
		}break;
	case WM_CLOSE:
		{
			if(MessageBox(0,L"Zamknac?", L"Pytanie!", MB_YESNO)==IDYES)
			{
				DestroyWindow(hwnd);
				break;
			}
		}break;

	case WM_PAINT:
		{
			GetClientRect(hwnd, &size);
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			//HDC mem = CreateCompatibleDC(hdc);
			//HBITMAP hBit = CreateCompatibleBitmap(hdc, size.right, size.bottom);
			//SelectObject(mem, hBit);
			//TextOutA(mem, x, 10, "TEST", 4);
			//BitBlt(hdc, 0, 0, size.right, size.bottom, mem, 0, 0, SRCCOPY);
			//DeleteDC(mem);
			//DeleteObject(hBit);
			EndPaint(hwnd, &ps);
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

bool Rejestruj(WNDCLASSEX &wc){
	wc.cbClsExtra=0;
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.hCursor=0;
	wc.hIcon=0;
	wc.hIconSm=0;
	wc.hInstance=global;
	wc.lpfnWndProc=WndProc;
	wc.lpszClassName=ClassName;
	wc.lpszMenuName=0;
	wc.style= CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClassEx(&wc))
		return false;
	else
		return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR Linia, int iShowCmd){
	global = hInstance;

	WNDCLASSEX wc;

	if(!Rejestruj(wc)){
		MessageBox(0,L"BLAD REJESTRACJI!", L"BLAD!", 0);
		return 1;
	}

	HWND okno = CreateWindowEx(0,ClassName,L"TEST", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 570 , 280, 0, 0, hInstance, 0);
	ShowWindow(okno, iShowCmd);
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(ClassName, hInstance);

	return msg.wParam;
}