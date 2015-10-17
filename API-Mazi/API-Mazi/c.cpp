#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
 
enum Operacja
{
        none,
        suma,
        roznica, 
        iloczyn,
        iloraz
};
 
 
std::string Oblicz(std::string, std::string, Operacja);
 
std::string inttostring(const int i)
{
        std::ostringstream ss;
        ss << i;
        return ss.str();
}
 
std::string OperacjaToString(const Operacja);
 
 
LPSTR NazwaKlasy = "KlasaOkienka";
MSG Komunikat;
 
HWND hwnd;
HWND button[16];
HWND label;
 
Operacja operacja = none; 
std::string liczby[2];
 
unsigned int obecnaLiczba = 0;
 
LPSTR buttonName[] = { "0", "+", "-", "=", "1", "2", "3", "*", "4", "5", "6", "/", "7", "8", "9", "<" };
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void MouseClick(HWND);
HWND getButton(LPSTR);
 
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = 0;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = NazwaKlasy;
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
 
        if (RegisterClassEx(&wc))
        {
 
                hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, "Calc", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 270, 400, NULL, NULL, hInstance, NULL);
 
                for (unsigned int x = 0; x < 4; ++x)
                        for (unsigned int y=0; y < 4;++y)
                                button[4*x+y] = CreateWindowEx(0, "BUTTON", buttonName[4*x+y], WS_CHILD | WS_VISIBLE ,10+(60*y), 290 - (60 *x), 50, 50, hwnd, NULL, hInstance, NULL);
 
 
                label = CreateWindowEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE, 10, 10, 230, 80, hwnd, NULL, hInstance, NULL);
                
                
                if (hwnd != NULL)
                {
                        ShowWindow(hwnd, nCmdShow);
                        UpdateWindow(hwnd);
 
                        while (GetMessage(&Komunikat, NULL, 0, 0))
                        {
                                TranslateMessage(&Komunikat);
                                DispatchMessage(&Komunikat);
                        }
                        return Komunikat.wParam;
                }
                else
                {
                        MessageBox(NULL, "B³¹d podczas tworzenia okna", "Sorry...", MB_ICONEXCLAMATION | MB_OK);
                        return 1;
                }
        }
        else
        {
                MessageBox(NULL, "B³¹d podczas rejestracji okna","Sorry...", MB_ICONEXCLAMATION | MB_OK);
                return 1;
        }
 
 
        return 0;
}
 
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
        switch( msg )
    {
    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;
       
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
        case WM_COMMAND:
                {
                        MouseClick((HWND)lParam);
                }break;
       
        default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    }
   
        return DefWindowProc(hwnd, msg, wParam, lParam);
}
 
void MouseClick(HWND button)
{       
        if (button == getButton("="))
        {
                liczby[0] = Oblicz(liczby[0], liczby[1], operacja);     
                liczby[1] = "";
                SetWindowText(label, (LPSTR)(liczby[0] + OperacjaToString(operacja) + liczby[1]).data());
                return;
        }
 
        if (button == getButton("+"))
        {               
                operacja = suma;
                obecnaLiczba = 1;
                SetWindowText(label, (LPSTR)(liczby[0] + OperacjaToString(operacja) + liczby[1]).data());
                return;
        }
        if (button == getButton("-"))
        {
                operacja = roznica;
                obecnaLiczba = 1;
                SetWindowText(label, (LPSTR)(liczby[0] + OperacjaToString(operacja) + liczby[1]).data());
                return;
        }
        if (button == getButton("*"))
        {
                operacja = iloczyn;
                obecnaLiczba = 1;
                SetWindowText(label, (LPSTR)(liczby[0] + OperacjaToString(operacja) + liczby[1]).data());
                return;
        }
 
        if (button == getButton("/"))
        {
                operacja = iloraz;
                obecnaLiczba = 1;
                SetWindowText(label, (LPSTR)(liczby[0] + OperacjaToString(operacja) + liczby[1]).data());
                return;
        }
                
        for (unsigned int i = 0; i < 10; ++i)
        {               
                if (button == getButton((LPSTR)inttostring(i).data()))
                {                       
                        liczby[obecnaLiczba] += inttostring(i);                 
                        SetWindowText(label, (LPSTR)(liczby[0]+OperacjaToString(operacja)+liczby[1]).data());
                        break;
                }
        }
}
 
HWND getButton(LPSTR name)
{       
        for (unsigned int i = 0; i < 16; ++i)
                if (!strcmp(name, buttonName[i]))
                {       
                        return button[i];
                }
        return NULL;
}
 
std::string OperacjaToString(const Operacja opercaja)
{
        switch (opercaja)
        {
        case suma:
                return " + ";
                break; 
        case iloczyn:
                return " * ";
                break;
        case iloraz:
                return " / ";
                break;
        case roznica:
                return " - ";
                break;
        }
 
        return "";
}
 
std::string Oblicz(std::string s1, std::string s2, Operacja opercaja)
{
        switch (opercaja)
        {
        case suma:
                return inttostring(atoi(s1.c_str()) + atoi(s2.c_str()));
                break;
        case iloczyn:
                return inttostring(atoi(s1.c_str()) * atoi(s2.c_str()));
                break;
        case iloraz:
                return inttostring(atoi(s1.c_str()) / atoi(s2.c_str()));
                break;
        case roznica:
                return inttostring(atoi(s1.c_str()) - atoi(s2.c_str()));
                break;
        }
 
        return "";
}