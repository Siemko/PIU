#include <windows.h>
#include <vector>
#include <time.h>
#include <algorithm>
 
using std::vector;
////////////////////////////
HINSTANCE glob_hInst;
const TCHAR nazwa[] = L"Klasa";

vector<HWND> Okna;
HWND hButton;

HDC hdc;
PAINTSTRUCT ps;
int k = 0;
int wymiarx, wymiary;
 
////////////////////////////
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
        switch(msg)
		{
                case WM_LBUTTONDOWN:
                {
                        if(Okna.size() < 5)
						{
								wymiarx = rand()%500;
								wymiary = rand()%500;
                                Okna.push_back(CreateWindowEx(0, nazwa, L"Okno", WS_VISIBLE | WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, 40*k,40*k, wymiarx, wymiary, 0, 0, glob_hInst, 0 ));
						}
                        else
						{
                                MessageBox(hwnd, L"ile okien !", L"Blad!", MB_OK | MB_ICONERROR);
						}
                }break;
				case WM_ERASEBKGND:
					{
						return 0;
					}break;
				case WM_PAINT:
					{
								hdc = BeginPaint(hwnd, &ps);
								HBRUSH pedzel[5];
								HBRUSH pudelko, pudelko2, pudelko3, pudelko4, pudelko5;
								pedzel[0] = CreateSolidBrush(RGB(255,0,0));
								pedzel[1] = CreateSolidBrush(RGB(0,255,0));
								pedzel[2] = CreateSolidBrush(RGB(0,0,255));
								pedzel[3] = CreateSolidBrush(RGB(0,0,0));
								pedzel[4] = CreateSolidBrush(RGB(255,255,255));
								pudelko =( HBRUSH ) SelectObject( hdc, pedzel[0] );
								pudelko2 =( HBRUSH ) SelectObject( hdc, pedzel[1] );
								pudelko3 =( HBRUSH ) SelectObject( hdc, pedzel[2] );
								pudelko4 =( HBRUSH ) SelectObject( hdc, pedzel[3] );
								pudelko5 =( HBRUSH ) SelectObject( hdc, pedzel[4] );
								SelectObject(hdc, pedzel[k]);
								Rectangle(hdc, 0, 0, 1000, 1000);
								DeleteObject(pedzel[k]);
								k++;
								EndPaint(hwnd, &ps);
					}break;

                case WM_RBUTTONDOWN:
                {
                        for(unsigned int i = 0; i < Okna.size() ; i++)
                        {
                                if(Okna[i] != hwnd)
                                {
                                        DestroyWindow( Okna[i] );
                                        Okna.erase( Okna.begin() + i );
                                        i--;
										k=1;
                                }
                        }       
                }break;
                
                case WM_CLOSE:
                {
                        if(Okna.size() == 1) MessageBox( hwnd, L"Kuniec", L"Bleeee", MB_OK | MB_ICONINFORMATION );
                        
                        for(unsigned int i = 0; i < Okna.size() ; i++)
                        {
                                if(Okna[i] == hwnd)
                                {
                                        DestroyWindow( Okna[i] );
                                        Okna.erase( Okna.begin() + i );
                                        break;
                                }
                        }
                        
                        if(Okna.size() == 0) PostQuitMessage( 0 );      
                }
                default: return DefWindowProc(hwnd, msg, wParam, lParam );
        }
        return 0;
}
 
void Rejestruj(WNDCLASSEX& wc)
{
        wc.hInstance = glob_hInst;
        wc.lpszClassName = nazwa;
        wc.lpfnWndProc = WndProc;
        wc.style = 0;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
        wc.hIcon = 0;
        wc.hIconSm = 0;
        wc.hCursor = 0;
        wc.lpszMenuName = 0;
        wc.cbWndExtra = 0;
        wc.cbClsExtra = 0;
        
        RegisterClassEx( &wc );
}
 
 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
        glob_hInst = hInstance;
        
        WNDCLASSEX wc;
        
        Rejestruj( wc );
        
		srand(time(NULL));
		wymiarx = rand()%500;
		wymiary = rand()%500;
        Okna.push_back(CreateWindowEx(0, nazwa, L"Okno", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 40,40, wymiarx, wymiary, 0, 0, hInstance, 0 ));

        MSG msg;

        while(GetMessage(&msg,0,0,0))
        {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
        }
        
        UnregisterClass( nazwa, hInstance );
        
 
        return msg.wParam;
}