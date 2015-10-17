#include <Windows.h>

using namespace std;

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

HWND klik1,klik2,klik3,klik4,edit1,edit2,edit3;
HINSTANCE hInstance;
float liczba,liczba1,wynik;
TCHAR wyn[10];
TCHAR szClassName[ ] = L"WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nFunsterStil)

{
	HWND hwnd;
	MSG messages;

	WNDCLASSEX wincl;
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof (WNDCLASSEX);
	wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	if (!RegisterClassEx (&wincl)) return 0;

	hwnd = CreateWindowEx ( 0, szClassName, L"Windows App", WS_BORDER, 500, 200, 300, 400, HWND_DESKTOP, NULL, hThisInstance, NULL );

	ShowWindow (hwnd, nFunsterStil);
	while (GetMessage (&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		klik1=CreateWindowEx(0,L"BUTTON",L"+",WS_CHILD|WS_VISIBLE,10,150,100,40,hwnd,(HMENU)1,hInstance,NULL);
		klik2=CreateWindowEx(0,L"BUTTON",L"-",WS_CHILD|WS_VISIBLE,10,200,100,40,hwnd,(HMENU)2,hInstance,NULL);
		klik3=CreateWindowEx(0,L"BUTTON",L"*",WS_CHILD|WS_VISIBLE,10,250,100,40,hwnd,(HMENU)3,hInstance,NULL);
		klik4=CreateWindowEx(0,L"BUTTON",L"/",WS_CHILD|WS_VISIBLE,10,300,100,40,hwnd,(HMENU)4,hInstance,NULL);
		edit1=CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"",WS_CHILD|WS_VISIBLE,10,10,270,35,hwnd,NULL,hInstance,NULL);
		edit2=CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"",WS_CHILD|WS_VISIBLE,10,60,270,35,hwnd,NULL,hInstance,NULL);
		edit3=CreateWindowEx(WS_EX_CLIENTEDGE,L"EDIT",L"",WS_CHILD|WS_VISIBLE,10,110,270,35,hwnd,NULL,hInstance,NULL);
		break;
	case WM_COMMAND:
		switch(wParam)
		{
		case 1:
			{
				DWORD dl1;
				dl1 = GetWindowTextLength(edit1);
				LPWSTR bf1= (LPWSTR)HeapAlloc(GetProcessHeap(), 0x1 | 0x8, dl1 + 1);
				GetWindowText(edit1, bf1, dl1 + 1);
				liczba = _wtof(bf1);
				dl1 = GetWindowTextLength(edit2);
				GetWindowText(edit2, bf1, dl1 + 1);
				liczba1 = _wtof(bf1);
				wynik=liczba+liczba1;
				wsprintf(wyn, L"%d", wynik);
				SetWindowText(edit3,wyn);
			}
			break;

		case 2:
			{
				DWORD dl1;
				dl1 = GetWindowTextLength(edit1);
				LPWSTR bf1= (LPWSTR)HeapAlloc(GetProcessHeap(), 0x1 | 0x8, dl1 + 1);
				GetWindowText(edit1, bf1, dl1 + 1);
				liczba = _wtof(bf1);
				dl1 = GetWindowTextLength(edit2);
				GetWindowText(edit2, bf1, dl1 + 1);
				liczba1 = _wtof(bf1);
				wynik=liczba-liczba1;
				wsprintf(wyn, L"%d", wynik);
				SetWindowText(edit3,wyn);
			}
			break;

		case 3:
			{
				DWORD dl1;
				dl1 = GetWindowTextLength(edit1);
				LPWSTR bf1= (LPWSTR)HeapAlloc(GetProcessHeap(), 0x1 | 0x8, dl1 + 1);
				GetWindowText(edit1, bf1, dl1 + 1);
				liczba = _wtof(bf1);
				dl1 = GetWindowTextLength(edit2);
				GetWindowText(edit2, bf1, dl1 + 1);
				liczba1 = _wtof(bf1);
				wynik=liczba*liczba1;
				wsprintf(wyn, L"%d", wynik);
				SetWindowText(edit3,wyn);
			}
			break;

		case 4:
			{
				DWORD dl1;
				dl1 = GetWindowTextLength(edit1);
				LPWSTR bf1= (LPWSTR)HeapAlloc(GetProcessHeap(), 0x1 | 0x8, dl1 + 1);
				GetWindowText(edit1, bf1, dl1 + 1);
				liczba = _wtof(bf1);
				dl1 = GetWindowTextLength(edit2);
				GetWindowText(edit2, bf1, dl1 + 1);
				liczba1 = _wtof(bf1);
				wynik=liczba/liczba1;
				wsprintf(wyn, L"%d", wynik);
				SetWindowText(edit3,wyn);
			}
			break;

		}
		break;
	case WM_DESTROY:
		PostQuitMessage (0);
		break;
	default:
		return DefWindowProc (hwnd, message, wParam, lParam);
	}
	return 0;
}