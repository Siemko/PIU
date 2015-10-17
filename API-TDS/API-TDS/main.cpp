#include <Windows.h>
#include <Windowsx.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#define _T TEXT

TCHAR ClassName[] = L"KlasaOkien";

template<class T>
const std::wstring LtoS(const T i)
{
	std::wstringstream buff;
	std::wstring wynik;
	buff << i;
	buff >> wynik;
	return wynik;
};


enum kierunek {up, down, left, right};

enum bron {karabin, pistolet};

class Gracz{
private:
	int x; int y;
	int speed;
	int rodzaj_broni;
	int magazynek;
public:
	Gracz(int _x, int _y, int _s, int _r): x(_x), y(_y), speed(_s), rodzaj_broni(_r)
	{
		if(rodzaj_broni == karabin)
			magazynek = 30;
		else
			magazynek = 12;
	}
	void Rysuj(HDC hdc)
	{
		HBRUSH br = CreateSolidBrush(RGB(0,0,128));
		SelectObject(hdc, br);
		Rectangle(hdc, x, y, x+10, y+10);
		DeleteObject(br);
	}
	bool Rusz(int kierunek)
	{
		switch(kierunek)
		{
		case up: y-=speed; return true; break;
		case down: y+=speed; return true; break;
		case right: x+=speed; return true; break;
		case left: x-=speed; return true; break;
		}
	}
	int GetRodzaj()
	{
		return rodzaj_broni;
	}
	RECT GetRect()
	{
		RECT gracz;
		gracz.top = y;
		gracz.left = x;
		gracz.bottom = y+10;
		gracz.right = y+10;
		return gracz;
	}
	POINT GetPos()
	{
		POINT pos;
		pos.x = x + 5;
		pos.y = y + 5;
		return pos;
	}
	int GetMagazynek()
	{
		return magazynek;
	}
	void Przeladuj()
	{
		if(rodzaj_broni == karabin)
			magazynek = 30;
		else
			magazynek = 12;
	}
	int operator--() {return magazynek--;}
}Player(0,0,2, pistolet);

class Pocisk{
private:
	POINT target;
	POINT pos;
	int speed;
public:
	void Rysuj(HDC hdc)
	{
		Ellipse(hdc, pos.x, pos.y, pos.x+3, pos.y+3);
	}
	Pocisk(int xx, int yy, int s) : speed(s)
	{ 
		pos.x = xx;
		pos.y = yy;
		target.x = pos.x; target.y = pos.y;
	}
	POINT operator()(int nx, int ny)
	{
		target.x = nx;
		target.y = ny;
		return pos;
	}
	bool Strzel()
	{
		if(target.x > pos.x) pos.x += speed;
		else if(target.x < pos.x) pos.x -= speed;
		if(target.y > pos.y) pos.y += speed;
		else if(target.y < pos.y) pos.y -= speed;
		return true;
	}
};

std::vector<Pocisk> pociski;
std::vector<POINT> cele;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		{
			SetTimer(hwnd, 832, 50, NULL);
			SetTimer(hwnd, 842, 100, NULL);
			pociski.push_back(Pocisk(-1200,-1200, 0));
			POINT k = {-1200,-1200};
			cele.push_back(k);
		}break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 832: InvalidateRect(hwnd, NULL, true);
			case 842:
				for(int i = 0; i < pociski.size(); i++)
				{
					pociski[i].Strzel();
				}
			}
		}break;
	case WM_LBUTTONDOWN:
		{

			POINT mouse;
			mouse.x = GET_X_LPARAM(lParam);
			mouse.y = GET_Y_LPARAM(lParam);
			if(Player.GetRodzaj() == karabin && Player.GetMagazynek()>0)
			{
				pociski.push_back(Pocisk(Player.GetPos().x, Player.GetPos().y, 5));
				cele.push_back(mouse);
				Player--;
			}
			else if(Player.GetRodzaj() == pistolet && Player.GetMagazynek()>0)
			{
				pociski.push_back(Pocisk(Player.GetPos().x, Player.GetPos().y, 2));
				cele.push_back(mouse);
				Player--;
			}
			for(int i = 0; i < pociski.size(); i++)
			{
				pociski[i](cele[i].x, cele[i].y);
			}
		}break;
	case WM_PAINT:
		{
			RECT size;
			GetClientRect(hwnd, &size);
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 10, 10, LtoS(Player.GetMagazynek()).c_str(), LtoS(Player.GetMagazynek()).length());
			Player.Rysuj(hdc);
			for(int i = 0; i < pociski.size(); ++i)
				pociski[i].Rysuj(hdc);
			EndPaint(hwnd, &ps);
		}break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case 'W':
				Player.Rusz(up); break;
			case 'S':
				Player.Rusz(down); break;
			case 'A':
				Player.Rusz(left); break;
			case 'D':
				Player.Rusz(right); break;
			case 'R':
				Player.Przeladuj(); break;
			}
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

	HWND hWnd = CreateWindowExW(NULL, ClassName, L"TDS", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 500, NULL, NULL, hInstance, NULL);

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