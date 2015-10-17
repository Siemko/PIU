#include <Windows.h>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <string>
#define DWAPI 6.28
using namespace std;
TCHAR ClassName[] = L"KlasaOkien";
HINSTANCE glob_hInst;
RECT size;

template <class T>
const wstring LtoS(T i)
{
	wstringstream buff;
	wstring wynik;
	
	buff << i << "px";
	buff >> wynik;
	
	return wynik;
}
int zakres_x = 10;
POINT apt[10];
static int cxClient, cyClient;
static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT:
		{
			RECT rY;
			rY.bottom = 40;
			rY.left = cxClient/2;
			rY.right = cxClient/2+50;
			rY.top = 0;
			RECT rX;
			rX.bottom = cyClient/2 + 50;
			rX.left = cxClient-50;
			rX.right = cxClient;
			rX.top = cyClient/2 + 10;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			MoveToEx(hdc, 0, cyClient/2, NULL);
			LineTo(hdc, cxClient, cyClient/2);
			MoveToEx(hdc, cxClient/2, cyClient, NULL);
			LineTo(hdc, cxClient/2, 0);
			DrawText(hdc, LtoS(cyClient).c_str(), -1, &rY, DT_CENTER);
			DrawText(hdc, LtoS(cxClient).c_str(), -1, &rX, DT_CENTER);
			MoveToEx(hdc, 0, cyClient/2, NULL);
			for(int i = 0; i<zakres_x; ++i)
			{
				apt[i].x = i * cxClient / zakres_x;
				apt[i].y = (int) (cyClient/2 * 1 - (sin(DWAPI * i / zakres_x)));

				Polyline(hdc, apt, zakres_x);
			}
			EndPaint(hWnd, &ps);
		}break;
	case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
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
	glob_hInst = hInstance;
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

	HWND Okno = CreateWindowEx(0, ClassName, L"Wykresy", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500,500, NULL, NULL, hInstance, NULL);
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