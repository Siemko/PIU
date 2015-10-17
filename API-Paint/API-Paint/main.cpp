#include <Windows.h>
#include <string>

TCHAR ClassName[] = L"KlasaOkien";
RECT user_size;
int kolory[18] =
{
	0x000000,
	0xFFFFFF,
	0x7F7F7F,
	0xFF0000,
	0xFF7F00,
	0xFF7F7F,
	0xFF007F,
	0xFF00FF,
	0XFFFF00,
	0x7F0000,
	0x7F7F00,
	0x7F007F,
	0x7FFF00,
	0x7F00FF,
	0x007F00,
	0x00FFFF,
	0x00FF00,
	0x0000FF
};
class Kwadrat{
	friend class Przybornik;
protected:
	POINT left_top;
	POINT right_bottom;
	int color;
public:
	Kwadrat()
	{
		left_top.x = 0;
		left_top.y = 0;
		right_bottom.x = 0;
		right_bottom.y = 0;
		color = kolory[0];
	}
	Kwadrat(int x1, int y1, int x2, int y2, int col)
	{
		left_top.x = x1;
		left_top.y = y1;
		right_bottom.x = x2;
		right_bottom.y = y2;
		color = col;
	}
	void Rysuj(HDC hdc)
	{
		HBRUSH kolor = CreateSolidBrush(color);
		SelectObject(hdc, kolor);
		Rectangle(hdc, left_top.x, left_top.y, right_bottom.x, right_bottom.y);
		DeleteObject(kolor);
	}
	bool PorPoz(int x, int y)
	{
		if(x>=left_top.x && x<=right_bottom.x && y>=left_top.y && y<=right_bottom.y)
			return true;
		else return false;
	}
	int retCol()
	{
		return color;
	}
};

class PedzelTyp : public Kwadrat{
protected:
	int ksztalt; // 1 - kwadrat | 2 - ko³o | 3 - trójk¹t
public:
	PedzelTyp(int x1, int y1, int x2, int y2, int col, int ksz): Kwadrat(x1, y1, x2, y2, col), ksztalt(ksz) {};
	PedzelTyp()
	{
		Kwadrat();
		ksztalt = 1;
	}
	void Rysuj(HDC hdc)
	{
		switch(ksztalt)
		{
		default:
			{
				HBRUSH kolor = CreateSolidBrush(color);
				HBRUSH nic = (HBRUSH)GetStockObject(WHITE_BRUSH);
				SelectObject(hdc, nic);
				Rectangle(hdc, left_top.x, left_top.y, right_bottom.x, right_bottom.y);
				DeleteObject(nic);
				SelectObject(hdc, kolor);
				Rectangle(hdc, left_top.x+5, left_top.y+5, right_bottom.x-5, right_bottom.y-5);
				DeleteObject(kolor);
			}break;
		case 2:
			{
				HBRUSH kolor = CreateSolidBrush(color);
				HBRUSH nic = (HBRUSH)GetStockObject(WHITE_BRUSH);
				SelectObject(hdc, nic);
				Rectangle(hdc, left_top.x, left_top.y, right_bottom.x, right_bottom.y);
				DeleteObject(nic);
				SelectObject(hdc, kolor);
				Ellipse(hdc, left_top.x+5, left_top.y+5, right_bottom.x-5, right_bottom.y-5);
				DeleteObject(kolor);
			}break;
		case 3:
			{
				HBRUSH kolor = CreateSolidBrush(color);
				HBRUSH nic = (HBRUSH)GetStockObject(WHITE_BRUSH);
				SelectObject(hdc, nic);
				Rectangle(hdc, left_top.x, left_top.y, right_bottom.x, right_bottom.y);
				DeleteObject(nic);
				SelectObject(hdc, kolor);
				POINT wierzcholki [3];
				wierzcholki[0].x = (right_bottom.x + left_top.x)/2;
				wierzcholki[0].y = left_top.y+5;
				wierzcholki[1].x = left_top.x+5;
				wierzcholki[1].y = right_bottom.y-5;
				wierzcholki[2].x = right_bottom.x-5;
				wierzcholki[2].y = right_bottom.y-5;
				Polygon(hdc, wierzcholki, 3);
				DeleteObject(kolor);
			}break;
		}
	}
	void Rysujb(HDC hdc)
	{
		switch(ksztalt)
		{
		default:
			{
				HBRUSH kolor = CreateSolidBrush(color);
				SelectObject(hdc, kolor);
				Rectangle(hdc, left_top.x+5, left_top.y+5, right_bottom.x-5, right_bottom.y-5);
				DeleteObject(kolor);
			}break;
		case 2:
			{
				HBRUSH kolor = CreateSolidBrush(color);
				SelectObject(hdc, kolor);
				Ellipse(hdc, left_top.x+5, left_top.y+5, right_bottom.x-5, right_bottom.y-5);
				DeleteObject(kolor);
			}break;
		case 3:
			{
				HBRUSH kolor = CreateSolidBrush(color);
				SelectObject(hdc, kolor);
				POINT wierzcholki [3];
				wierzcholki[0].x = (right_bottom.x + left_top.x)/2;
				wierzcholki[0].y = left_top.y+5;
				wierzcholki[1].x = left_top.x+5;
				wierzcholki[1].y = right_bottom.y-5;
				wierzcholki[2].x = right_bottom.x-5;
				wierzcholki[2].y = right_bottom.y-5;
				Polygon(hdc, wierzcholki, 3);
				DeleteObject(kolor);
			}break;
		}
	}
	int retKsz()
	{
		return ksztalt;
	}
};

class Przybornik{
protected:
	RECT poz;
	int color;
	Kwadrat PaletaKolorow[18];
	PedzelTyp Pedzle[3];
public:
	Przybornik(int col)
	{
		color = col;
	}
	void RysujPalete(HDC hdc)
	{
		for(int i = 0; i<18; ++i)
		{
			if(i%2==1)
			{
				PaletaKolorow[i] = Kwadrat(poz.left+15, poz.top+(i*20), poz.left+45, poz.top+30+(i*20), kolory[i]);
			}
			else
			{
				PaletaKolorow[i] = Kwadrat(poz.left+75, poz.top+20+(i*20), poz.left+105, poz.top+50+(i*20), kolory[i]);
			}
		}
		for(int i=0; i<18; ++i)
			PaletaKolorow[i].Rysuj(hdc);
		RECT last_el;
		last_el.top = PaletaKolorow[17].left_top.y;
		last_el.bottom = PaletaKolorow[17].right_bottom.y;
		last_el.left = PaletaKolorow[17].left_top.x;
		last_el.right = PaletaKolorow[17].right_bottom.x;
		Pedzle[0] = PedzelTyp(last_el.left, last_el.top+100, last_el.right, last_el.bottom+100, kolory[0], 1);
		Pedzle[0].Rysuj(hdc);
		Pedzle[1] = PedzelTyp(last_el.left+40, last_el.top+100, last_el.right+40, last_el.bottom+100, kolory[0], 2);
		Pedzle[1].Rysuj(hdc);
		Pedzle[2] = PedzelTyp(last_el.left+80, last_el.top+100, last_el.right+80, last_el.bottom+100, kolory[0], 3);
		Pedzle[2].Rysuj(hdc);
	}
	void Rysuj(HWND hwnd, HDC hdc)
	{
		HBRUSH kolor = CreateSolidBrush(color);
		SelectObject(hdc, kolor);
		GetClientRect(hwnd, &user_size);
		poz.left = user_size.right-150;
		poz.bottom = user_size.bottom;
		poz.right = user_size.right;
		poz.top = user_size.top;
		Rectangle(hdc, poz.left, poz.top, poz.right, poz.bottom);
		DeleteObject(kolor);
		RysujPalete(hdc);
	}
	Kwadrat RetKwadrat(int i)
	{
		return PaletaKolorow[i];
	}
	PedzelTyp RetPedzel(int i)
	{
		return Pedzle[i];
	}

};
Przybornik* ref;

int glKolor = kolory[3];
int glKsztalt = 1;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			ref->Rysuj(hWnd, hdc);
			EndPaint(hWnd, &ps);
		}break;
	case WM_LBUTTONDOWN:
		{
			POINT pos;
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);
			for(int i =0; i<18; ++i)
			{
				if(ref->RetKwadrat(i).PorPoz(pos.x, pos.y)==true)
					glKolor = ref->RetKwadrat(i).retCol();
			}
			if(pos.x<user_size.right-150)
			{
				HDC hdc = GetDC(hWnd);
				PedzelTyp nowy(pos.x, pos.y, pos.x+50, pos.y+50, glKolor, glKsztalt);
				nowy.Rysujb(hdc);
				ReleaseDC(hWnd, hdc);
			}
		}break;

	case WM_RBUTTONDOWN:
		{
			POINT pos;
			pos.x = LOWORD(lParam);
			pos.y = HIWORD(lParam);
			for(int i=0; i<3; ++i)
			{
				if(ref->RetPedzel(i).PorPoz(pos.x, pos.y)==true)
					glKsztalt = ref->RetPedzel(i).retKsz();
			}
		}break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}break;
	default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
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

	HWND Okno = CreateWindowEx(0, ClassName, L"Paint", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600,600, NULL, NULL, hInstance, NULL);
	if(Okno==NULL)
	{
		MessageBox(NULL, L"ERROR: TWORZENIE OKNA", L"ERROR", NULL);
		return 1;
	}
	Przybornik glowny(0xCCCCCC);
	ref = &glowny;
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