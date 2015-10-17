#include <Windows.h>
#include <vector>
#include <time.h>
#include <algorithm>

TCHAR ClassName[] = L"KlasaOkien";
std::vector<HWND> okna;
HINSTANCE glob_hInst;
RECT size;
int i = 1;
static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT:
		{
			HBRUSH k = CreateSolidBrush(RGB(0,255,0));
			HBRUSH j = CreateSolidBrush(RGB(255,0,0));
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			MoveToEx(hdc,0,0,NULL);
			GetClientRect(hWnd, &size);
			if(i%2==1)
			{
				SelectObject(hdc, k);
				Rectangle(hdc, size.left,size.top,size.right,size.bottom);
			}
			else
			{
				SelectObject(hdc, j);
				Rectangle(hdc, size.left,size.top,size.right,size.bottom);
			}
			SaveDC(hdc);
			EndPaint(hWnd, &ps);
		}break;
	case WM_ERASEBKGND:
		{
			return 1;
		}
	case WM_LBUTTONDOWN:
		{
			int wymiarx = rand()%500+100;
			int wymiary = rand()%500+100;
			okna.push_back(CreateWindowEx(0, ClassName, L"Okno", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 40,40, wymiarx, wymiary, 0, 0, glob_hInst, 0 ));
			i++;
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
	wc.hbrBackground = NULL;
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