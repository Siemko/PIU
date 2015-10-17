#include <Windows.h>
#include <Windowsx.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#define _T TEXT

TCHAR ClassName[] = L"KlasaOkien";
int mnoznik = 2;
int points = 0;

template<class T>
const std::wstring LtoS(const T i)
{
	std::wstringstream buff;
	std::wstring wynik;
	buff << i;
	buff >> wynik;
	return wynik;
};

class Baloon{
private:
	bool click;
	POINT pos;
	int speed;
	int color;
	int size;
public:
	Baloon(int x, int y, int s, int c, int siz)
	{
		pos.x = x; pos.y = y; speed = s; color = c; size = siz; click = false;
	}
	int operator++()
	{
		if (speed<8) return speed++;
	};
	int operator--()
	{
		if (size>5) return size--;
	};
	bool move()
	{
		pos.y -= speed;
		return true;
	}
	bool retclick()
	{
		return click;
	}
	void draw(HDC hdc)
	{
		HBRUSH p = CreateSolidBrush(color);
		SelectObject(hdc, p);
		Ellipse(hdc, pos.x - size, pos.y - size, pos.x + size, pos.y + size);
		DeleteObject(p);
	}
	RECT retpos()
	{
		RECT bPos;
		bPos.left = pos.x - 3 * size;
		bPos.top = pos.y - 3 * size;
		bPos.right = pos.x + 3 * size;
		bPos.bottom = pos.y + 3 * size;
		return bPos;
	}
	bool PorPoz(int x, int y)
	{
		if (x >= retpos().left && x <= retpos().right && y >= retpos().top && y <= retpos().bottom)
			return true;
		else return false;
	}
	void clicked()
	{
		this->click = true;
	}
};

std::vector<Baloon> balony;
void Uzupelnij()
{
	for (int i = 0; i < rand() % 2000 + 300; ++i)
	{
		balony.push_back(Baloon(rand() % 900, 400 + (i * 50), 1, RGB(rand() % 255, rand() % 255, rand() % 255), 15));
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		SetTimer(hwnd, 832, 10, NULL);
		SetTimer(hwnd, 1200, 10000, NULL);
		Uzupelnij();
	}break;
	case WM_TIMER:
	{
		switch (wParam)
		{
		case 832:
		{
			for (int i = 0; i < balony.size(); ++i)
			{
				balony[i].move(); InvalidateRect(hwnd, &(balony[i].retpos()), true);
			}
		}break;
		case 1200:
		{
			for (int i = 0; i < balony.size(); ++i)
			{
				balony[i]++;
				balony[i]--;
			}
			mnoznik++;
		}break;
		}
	}
	case WM_LBUTTONDOWN:
	{

		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		for (int i = 0; i < balony.size(); ++i)
		{
			if (balony[i].PorPoz(xPos, yPos) && (!balony[i].retclick()))
			{
				points += mnoznik;
				balony[i].clicked();
				RECT gdzie;
				gdzie.bottom = 25;
				gdzie.top = 10;
				gdzie.left = 10;
				gdzie.right = 50;
				InvalidateRect(hwnd, &gdzie, true);
			}
		}
	}break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 10, 10, LtoS(points).c_str(), LtoS(points).length());
		for (int i = 0; i < balony.size(); ++i)
			balony[i].draw(hdc);
		EndPaint(hwnd, &ps);
	}break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}break;
	default: return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = ClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("B³¹d rejestracji klasy!"), _T("B³¹d"), NULL);
		return 1;
	}

	HWND hWnd = CreateWindowExW(NULL, ClassName, L"Balony", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 500, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("B³¹d tworzenia okna"), _T("B³¹d"), NULL);
		return 2;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ClassName, hInstance);
	return (int)msg.wParam;
}