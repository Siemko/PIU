#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
int p = 0;
int l = 0;
WCHAR napis[64];
LPCWSTR NazwaKlasy=L"Klasa Okienka";
MSG Komunikat;
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND g_hPrzycisk;
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
WNDCLASSEX wc;
wc.cbSize = sizeof(WNDCLASSEX);
wc.style = 0;
wc.lpfnWndProc=WndProc;
wc.cbClsExtra = 0;
wc.cbWndExtra = 0;
wc. hInstance = hInstance;
wc. hIcon = LoadIcon(NULL, IDI_APPLICATION);
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground=(HBRUSH)(COLOR_WINDOW + 1);
wc.lpszMenuName= NULL;
wc.lpszClassName = NazwaKlasy;
wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
if(!RegisterClassEx(&wc))
{
MessageBox( NULL, L"Nie otworzyles!", L"Niestety", MB_ICONEXCLAMATION | MB_OK );
return 1;
}
HWND hwnd;
hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,NazwaKlasy, L"Oto Okienko!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);
if(hwnd == NULL)
{
MessageBox( NULL, L"Tnie przyszlo.", L"lipa", MB_ICONINFORMATION);
return 1;
}
g_hPrzycisk=CreateWindowEx(0, L"BUTTON", L"Nasz przycisk", WS_CHILD | WS_VISIBLE, 100, 100, 100, 30, hwnd, NULL, hInstance, NULL);
g_hPrzycisk=CreateWindowEx(0, L"Button", L" Checkbox", WS_CHILD |WS_VISIBLE, 250,100,100,30,hwnd,NULL,hInstance,NULL);
g_hPrzycisk=CreateWindowEx( 0, L"BUTTON", L"Checkbox", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 100, 200, 150, 30, hwnd, NULL, hInstance, NULL );

ShowWindow(hwnd,nCmdShow);
UpdateWindow(hwnd);

while(GetMessage( &Komunikat, NULL, 0, 0))
{
TranslateMessage( &Komunikat);
DispatchMessage(&Komunikat);
}
return Komunikat.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
switch(msg)
{
case WM_COMMAND:
if((HWND) lParam == g_hPrzycisk);

MessageBox(hwnd,L"jebnal!",L"yeah",MB_ICONINFORMATION);

break;
case WM_LBUTTONDOWN:
	{
l++;
if(l%10==0){
MessageBox(hwnd,L"Wyczyscic zmienna?",L"ups...",MB_ICONINFORMATION);
g_hPrzycisk=CreateWindowEx(0, L"Button", L" Checkbox", WS_CHILD |WS_VISIBLE, 250,100,100,30,hwnd,NULL,0,NULL);
g_hPrzycisk=CreateWindowEx( 0, L"BUTTON", L"Checkbox", WS_CHILD | WS_VISIBLE, 100, 200, 150, 30, hwnd, NULL, 0, NULL );
l=0;
}
swprintf(napis, L"Lewy: %d Prawy: %d", l, p);
SetWindowText(hwnd, napis);
	}
break;
case WM_RBUTTONDOWN:
	{
p++;
if(p%10==0){
MessageBox(hwnd,L"Wyczyscic zmienna?",L"ups...",MB_ICONINFORMATION);
p=0;

}
swprintf(napis, L"Lewy: %d Prawy: %d", l, p);
SetWindowText(hwnd, napis);
	}
break;
case WM_CLOSE:
DestroyWindow(hwnd);
break;
case WM_DESTROY:
PostQuitMessage(0);
break;
default:
return DefWindowProc(hwnd,msg,wParam,lParam);
}
return 0;
}