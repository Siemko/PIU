#include <Windows.h>
#include <WindowsX.h>
#include <vector>


struct MouseEvent
{
	MouseEvent(int x, int y) : x(x), y(y){}
	int x;
	int y;
};

class Toolbox
{
public:
	Toolbox(){}
	Toolbox(int x, int y, int width, int height):
	x(x), y(y), width(width), height(height)
	{}
	void draw(HDC hdc)
	{
		HBRUSH brush = CreateSolidBrush(color);
		SelectObject( hdc, brush );

		Rectangle( hdc, x, y, x + width, y + height );

		DeleteObject( brush );
	}

	void set_x_y(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void set_width_height(int width, int height)
	{
		this->width = width;
		this->height = height;
	}

	void set_color(COLORREF c)
	{
		color = c;
	}

	void draw_pantone(HDC hdc, COLORREF c, int x, int y)
	{
		HBRUSH brush = CreateSolidBrush(c);
		SelectObject( hdc, brush );

		Rectangle( hdc, x, y, x + 50, y + 50 );

		DeleteObject( brush );
	}
private:
	COLORREF color;
	int x;
	int y;
	int width;
	int height;
};
class Field
{
public:
	Field(){}
	Field(int x, int y, int width, int height):
	x(x), y(y), width(width), height(height), pressed(0)
	{
		map.resize(width);
		for(int i = 0; i < map.size(); i++)
		{
			map[i].resize(height, RGB(255, 255, 255));
		}
	}

	void set_color(COLORREF c)
	{
		color = c;
	}

	void draw_pixel(int x, int y)
	{
		map[x][y] = color;
	}

	void press_event(MouseEvent ev)
	{
		pressed = true;
		draw_pixel(ev.x, ev.y);
		last_x = ev.x;
		last_y = ev.y;
	}

	void release_event(MouseEvent ev)
	{
		pressed = false;
	}

	void move_event(MouseEvent ev)
	{
		if(pressed)
		{
			draw_pixel(ev.x, ev.y);
			last_x = ev.x;
			last_y = ev.y;
		}
	}

	void paint(HDC hdc)
	{
		for(int i = 0; i < map.size(); ++i)
			for(int j = 0; j < map[i].size(); ++j) 
				SetPixel(hdc, x + i, y + j, map[i][j]);
	}

private:
	std::vector<std::vector<COLORREF>> map;
	COLORREF color;
	bool pressed;
	int x;
	int y;
	int width;
	int height;
	int last_x;
	int last_y;
};

Field *field;
Toolbox *toolbox;

class Application
{
public:
	Application()
	{
		wc.cbSize = sizeof( WNDCLASSEX );
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
		wc.hCursor = LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"Application";
		wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"ERROR: REJESTRACJA", L"ERROR", NULL);
		}
	}

	~Application()
	{
		UnregisterClass(L"Application", GetModuleHandle(NULL));
	}

	void create_window()
	{
		hwnd = CreateWindowEx(0, L"Application", L"Okienko", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600,600, NULL, NULL, GetModuleHandle(NULL), NULL);
		if(hwnd == NULL)
		{
			MessageBox(NULL, L"ERROR: Tworzenie okna", L"ERROR", NULL);
		}
	}

	void show_window()
	{
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);
	}

	
	static LRESULT CALLBACK WndProc(HWND hwndMain, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_PAINT:
			{
				HDC hdc = GetDC(hwndMain);
				field->paint(hdc);
				toolbox->draw(hdc);
				toolbox->draw_pantone(hdc, RGB(0, 204, 0), 500, 100);
				ReleaseDC(hwndMain, hdc);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				field->press_event(MouseEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			}
			break;
		case WM_LBUTTONUP:
			{
				field->release_event(MouseEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			}
			break;
		case WM_MOUSEMOVE:
			{
				field->move_event(MouseEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwndMain, uMsg, wParam, lParam);
		}
	}

	int run()
	{
		MSG msg;
		while(GetMessage(&msg, 0, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return static_cast<int>(msg.wParam);
	}
private:
	WNDCLASSEX wc;
	HWND hwnd;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application app;
	app.create_window();

	field = new Field(0, 0, 450, 600);
	field->set_color(0xCCCCCC);
	toolbox = new Toolbox(450, 0, 150, 600);
	toolbox->set_color(0xCCCCCC);
	app.show_window();
	return app.run();
}