#include <Windows.h>
#include <vector>
using std::vector;
TCHAR ClassName[] = L"KlasaOkien";

const UINT_PTR TIMER_ID = 8;
UINT TIMER_DELAY = 100;
vector <POINT> gwiazdki;

class Background{
private:
	bool narysowane;
public:
	Background(bool n) : narysowane(n) {};
	void PrepareBackground(HWND hwnd)
	{
		RECT size;
		GetClientRect(hwnd, &size);
		for (int y = 0; y < 30; ++y)
			for (int x = 0; x < 60; ++x)
			{
				POINT gw;
				gw.x = rand()%size.right - size.left;
				gw.y = rand() % size.bottom - size.top;
				gwiazdki.push_back(gw);
			}
	}
	void DrawBackground(HDC hdc)
	{
		for (int i = 0; i < gwiazdki.size(); ++i)
			SetPixel(hdc, gwiazdki[i].x, gwiazdki[i].y, RGB(rand() % 255, rand() % 255, rand() % 255));
	}
}back(false);

class Bullet{
private:
	POINT pos;
	int speed;
public:
	Bullet(int s) : speed(s) {};
	void move()
	{

	}
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		back.PrepareBackground(hwnd);
		SetTimer(hwnd, TIMER_ID, TIMER_DELAY, NULL);
	}break;
	case WM_PAINT:
	{
		RECT size;
		GetClientRect(hwnd, &size);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		//skalowanie!
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetWindowExtEx(hdc, 600, 600, NULL);
		SetViewportExtEx(hdc, size.right, size.bottom, NULL);
		back.DrawBackground(hdc);
		
		EndPaint(hwnd, &ps);
	}break;
	case WM_MOUSEMOVE:
	{
		POINT mouse;
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
	}break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;
	case WM_TIMER:
	{
		switch (wParam)
		{
		case TIMER_ID:
			InvalidateRect(hwnd, NULL, true);
		}
	
	} break;
	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = &WndProc;
	wc.lpszClassName = ClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("B³¹d rejestracji klasy!"), TEXT("B³¹d"), NULL);
		return 1;
	}
	HWND okno = CreateWindowEx(NULL, ClassName, L"Ace of SPACE", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 700, NULL, NULL, hInstance, NULL);
	if (!okno)
	{
		MessageBox(NULL, TEXT("B³¹d tworzenia okna!"), TEXT("B³¹d"), NULL);
		return 2;
	}
	ShowWindow(okno, nCmdShow);
	UpdateWindow(okno);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ClassName, hInstance);
	return (int)msg.wParam;
}