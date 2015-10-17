#include <windows.h>

MSG Komunikat;

class Tiles
{
private:
	int x;
	int y;
	int width;
	int height;
	HDC hdc;
public:
	Tiles(int x, int y, int width, int height):
	  x(x), y(y), width(width), height(height)
	  {}

	void draw(HDC hdc)
	{
		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdc, brush);
		Rectangle(hdc, x, y, x + width, y + height);
		DeleteObject(brush);
	}
}Test(0,0,30,30);

Tiles *tiles;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

	// WYPE£NIANIE STRUKTURY
	WNDCLASSEX wc;

	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Klasa";
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

	// REJESTROWANIE KLASY OKNA
	if( !RegisterClassEx( & wc ) )
	{
		MessageBox( NULL, L"Wysoka Komisja odmawia rejestracji tego okna!", L"Niestety...",
			MB_ICONEXCLAMATION | MB_OK );
		return 1;
	}

	// TWORZENIE OKNA
	HWND hwnd;

	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, L"Klasa", L"Oto okienko", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL );

	if( hwnd == NULL )
	{
		MessageBox( NULL, L"error", L"error", MB_ICONEXCLAMATION );
		return 1;
	}
	tiles = new Tiles(0, 0, 80, 40);

	ShowWindow( hwnd, nCmdShow ); // Poka¿ okienko...
	UpdateWindow( hwnd );


	// Pêtla komunikatów
	while( GetMessage( & Komunikat, NULL, 0, 0 ) )
	{
		TranslateMessage( & Komunikat );
		DispatchMessage( & Komunikat );
	}
	return Komunikat.wParam;
}

// OBS£UGA ZDARZEÑ
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			RECT screen;
			GetClientRect(hwnd, &screen);
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, 700, 500, 0);
			SetViewportExtEx(hdc, screen.right, screen.bottom, 0);
			
			Test.draw(hdc);
			//tiles->draw(hdc);
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_CLOSE:
		{
		DestroyWindow( hwnd );
		break;
		}
	case WM_DESTROY:
		{
		PostQuitMessage( 0 );
		break;
		}
	default:
		return DefWindowProc( hwnd, msg, wParam, lParam );
	}

	return 0;
}