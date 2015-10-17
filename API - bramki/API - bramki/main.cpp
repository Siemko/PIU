// Program tworzy 3 okna, kazde z mozliwoscia zamkniecia (pyta przed!)
// 1 okno - normalne, 2 okno - bez minimalizacji i maksymalizacji, 3 okno- bez mozliwosci powiekszenia.
// Na koniec info, ze program zakonczyl prace.
// Rejestracja klasy w podprogramie.


#include <Windows.h>
#include <WindowsX.h>
#include <vector>
#include <string>



bool to_bool(TCHAR string) { 
    return string == '1';
}



using namespace std;

TCHAR ClassName [] = L"Nazwa Klasy";
vector<HWND>okno;
HINSTANCE global;

DWORD dlugosc1;
DWORD dlugosc2;
LPWSTR Bufor1;
LPWSTR Bufor2;
vector<bool> we1;
vector<bool> we2;
int index;

std::vector<TCHAR*> Uzupelnij()
{
	std::vector<TCHAR*> nazwy;
	nazwy.push_back(L"NOT");
	nazwy.push_back(L"AND");
	nazwy.push_back(L"NAND");
	nazwy.push_back(L"OR");
	nazwy.push_back(L"NOR");
	nazwy.push_back(L"EXOR");
	nazwy.push_back(L"EXNOR");
	return nazwy;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
		}break;
	case WM_CLOSE:
		{
			if(MessageBox(0,L"Zamknac?", L"Pytanie!", MB_YESNO)==IDYES)
			{
				DestroyWindow(hwnd);
				break;
			}
		}break;
	case WM_DESTROY:
		GlobalFree( Bufor1 );
		GlobalFree( Bufor2 );
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case 300:
				{
					index=SendMessage(GetDlgItem(hwnd, 300),CB_GETCURSEL,0,0);
					dlugosc1 = GetWindowTextLength(GetDlgItem(hwnd,100));
					Bufor1 =( LPWSTR ) GlobalAlloc( GPTR, dlugosc1 + 1 );
					GetWindowText( GetDlgItem(hwnd,100), Bufor1, dlugosc1 + 1 );
					dlugosc2 = GetWindowTextLength(GetDlgItem(hwnd,101));
					Bufor2 =( LPWSTR ) GlobalAlloc( GPTR, dlugosc2 + 1 );
					GetWindowText( GetDlgItem(hwnd,101), Bufor1, dlugosc2 + 1 );
					for(int i = 0; i<dlugosc1; i++)
					{
						if(Bufor1[i] = L'1')
							we1.push_back(1);
						if(Bufor1[i] = L'0')
							we1.push_back(0);
					}
					for(int i = 0; i<dlugosc2; i++)
					{
						if(Bufor2[i] = L'1')
							we2.push_back(1);
						if(Bufor2[i] = L'0')
							we2.push_back(0);
					}
				}break;
			}break;
		}break;

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
	vector<TCHAR*> nazwy = Uzupelnij();
	global = hInstance;

	WNDCLASSEX wc;

	if(!Rejestruj(wc)){
		MessageBox(0,L"BLAD REJESTRACJI!", L"BLAD!", 0);
		return 1;
	}

	HWND okno = CreateWindowEx(0,ClassName,L"Kalkulator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 570 , 280, 0, 0, hInstance, 0);

	CreateWindowEx(0, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 70, 150, 20, okno, HMENU(100), hInstance, NULL );
	CreateWindowEx(0, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 110, 150, 20, okno, HMENU(101), hInstance, NULL );
	HWND hCombo = CreateWindowEx( WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
	CBS_DROPDOWN, 210, 90, 150, 200, okno, (HMENU)300, hInstance, NULL );
	CreateWindowEx(0, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 370, 90, 150, 20, okno, HMENU(102), hInstance, NULL );
	for(int i=0; i<nazwy.size(); i++)
	{
		SendMessage(hCombo, CB_ADDSTRING, 0,( LPARAM )nazwy[i]);
	}
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(ClassName, hInstance);

	return msg.wParam;
}