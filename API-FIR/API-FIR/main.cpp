#include <Windows.h>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <string>
#define DWAPI 6.28
using namespace std;
TCHAR ClassName[] = L"KlasaOkien";

RECT size;

template <class T>
const wstring LtoS(T i)
{
	wstringstream buff;
	wstring wynik;
	
	buff << i;
	buff >> wynik;
	
	return wynik;
}

void SetView(HWND hwnd, HDC hdc)
{
	GetClientRect(hwnd, &size);
	SetMapMode(hdc, MM_ISOTROPIC);
	SetViewportOrgEx(hdc, size.left, size.bottom/2, NULL);
	SetViewportExtEx(hdc, size.right, -size.bottom, NULL);
	SetWindowExtEx(hdc, 1200, 1200, NULL);
}

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetView(hWnd, hdc);
			HPEN pen = CreatePen(PS_SOLID, 5, RGB(0,0,0));
			SelectObject(hdc, pen);
			LineTo(hdc, 50, 0);
			Rectangle(hdc, 50, -20, 90, 20);
			MoveToEx(hdc, 89, 0, NULL);
			LineTo(hdc, 140, 0);
			DeleteObject(pen);
			EndPaint(hWnd, &ps);
		}break;
	case WM_LBUTTONDOWN:
		{

		}break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}break;
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = &wndProc;
	wc.lpszClassName = ClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"ERROR: REJESTRACJA", L"ERROR", NULL);
		return 1;
	}

	HWND Okno = CreateWindowEx(0, ClassName, L"Struktura kanoniczna", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500,500, NULL, NULL, hInstance, NULL);
	if(Okno==NULL)
	{
		MessageBox(NULL, L"ERROR: TWORZENIE OKNA", L"ERROR", NULL);
		return 1;
	}
	ShowWindow(Okno, nCmdShow);
	UpdateWindow(Okno);
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ClassName, hInstance);
	return static_cast<int>(msg.wParam);
}