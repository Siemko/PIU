#include <windows.h>
#include <windowsx.h> // par� makr st�d si� prdzydaje
#include <sstream>
#include <iostream>

UINT_PTR TIMER_ID = 8; // ID timera
UINT TIMER_DELAY = 10; // op�nienie timera 0.01 sekundy

int X_RES = 600;
int Y_RES = 600;

//
// Klasa objektu kt�ry �ledzi myszk�. 
//
class Object{
private:
	POINT Pos; //Aktualna pozycja obiektu
	POINT Target; // pozycja do kt�rego obiekt zmierza
	int Speed; // ilo�� pikseli o kt�re nast�puje skok.
public:
	POINT GetPos(){ return Pos; } //zwraca pozycj�
	int GetSpeed(){ return Speed;} // zwraca szybko��

	bool Draw(HDC k){ // metoda rysuj�ca obiekt. Kwadrat
		Rectangle(k, Pos.x-20, Pos.y-20, Pos.x+20, Pos.y+20);
		return true;
	}

	Object(int _X, int _Y, int S):Speed(S) {  //konstruktor
		Pos.x = _X;
		Pos.y = _Y;
		Target = Pos;
	}

	int operator++(){return ++Speed;} //operator: O++ zwi�ksza Speed

	int operator--(){return --Speed;} //operator O-- zmniejsza Speed

	POINT operator()(int X, int Y){ // operator funkcyjny(): O(12, 13) -> ustawia Target na X=12, Y=13. Przyk�ad na linii 98
		Target.x = X;
		Target.y = Y;
		return Pos;
	}

	bool Move(){ //czyli zmie� pozycj� w stron� celu (TARGET) o pewn� delte (SPEED)
		if(Target.x > Pos.x) Pos.x += Speed;
		else if(Target.x < Pos.x) Pos.x -= Speed;

		if(Target.y > Pos.y) Pos.y += Speed;
		else if(Target.y < Pos.y) Pos.y -= Speed;

		return true;
	}

}Ship(30, 30, 1); // tworzenie obiektu.

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam){
	switch(msg){
		case WM_PAINT:{ 		
			PAINTSTRUCT lp; //struktura wymagana dla BeginPaint i EndPaint
			HDC Kont = BeginPaint(hWnd, &lp);		

			//Skalowanie nast�pne 5 linijek.
			RECT screen;
			GetClientRect(hWnd, &screen);
			SetMapMode(Kont, MM_ANISOTROPIC);
			SetWindowExtEx(Kont, X_RES, Y_RES, 0);
			SetViewportExtEx(Kont, screen.right, screen.bottom, 0);
			
			//rysuj statek
			Ship.Draw(Kont);	
			
			//Poka� aktualn� pozycj� statku w lewym g�rnym rogu. 
			std::wstringstream pos;
			pos << L"POS: " << Ship.GetPos().x << L", " << Ship.GetPos().y;
			TextOut(Kont, 10, 10, pos.str().c_str(), pos.str().length()); 
			
			EndPaint(hWnd, &lp);
			DefWindowProc(hWnd, msg, wparam, lparam);
			break;
		}case WM_TIMER:{ //obs�uga timera.
			if(wparam = TIMER_ID){ //Jest tylko jeden timer wi�c wszystkie WM_TIMER powinny pochodzi� od tego jednego
						//Dodane na wypadek kilku timer�w. Pewnie switch(wparam){ case TIMER_ID: ...} by�by lepszy dla kilku timer�w.
				RECT screen;
				GetClientRect(hWnd, &screen);
				Ship.Move(); //rusz statkiem, czyli tylko zmie� pozycj�
				InvalidateRect(hWnd, &screen, true); // przerysuj zawarto�� okna. 		
			}
			break;
		}case WM_MOUSEMOVE:{ //obs�uga poruszania mysz�. 
			POINT p[1] = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
			//GET_X_LPARAM i GET_Y_LPARAM pochodz� z windowsx.h i pozawalaj� na wyci�gni�cie wsp�rz�dnych z lparam bez dupcenia si� z 
			// przesuwaniem bitowym.
			
			//HDC Kont = GetDC(hWnd); //wat is this i don't even
			//DPtoLP(Kont, p, 1);  //wat is this i don't even
			
			Ship(p[0].x, p[0].y); //ustaw pozycj� myszki na nowy cel
			//ReleaseDC(hWnd, Kont); 
			break;
		}case WM_LBUTTONDOWN:{	// lewy guzik myszki zmniejsza delay dla timera, co przy�piesza ruchy obiektu. Czas jest w tysi�cznych sekundy (10^-3)
			if(TIMER_DELAY > 10)
				TIMER_DELAY -= 10;
			SetTimer(hWnd, TIMER_ID, TIMER_DELAY, 0); 
			break;
		}case WM_RBUTTONDOWN:{ // prawy guzik zwi�ksza delay dla timera, obiekt spowalnia. Reszta tak jak wy�ej. 	
			if(TIMER_DELAY < 1000)
				TIMER_DELAY += 10;
			SetTimer(hWnd, TIMER_ID, TIMER_DELAY, 0);
			break;
		}case WM_DESTROY:{
			PostQuitMessage(0);
			break;
		}
		default: return DefWindowProc(hWnd, msg, wparam, lparam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd){
	
	TCHAR klasa[] = TEXT("Klasa");
	TCHAR app[] = TEXT("BEG");
	WNDCLASSEX wndClass = {sizeof(WNDCLASSEX), CS_VREDRAW|CS_HREDRAW, WndProc, 0, 0, hInstance, 0, 0, (HBRUSH)(COLOR_WINDOW+1), 0, klasa, 0};

	if(!RegisterClassEx(&wndClass)){
		MessageBox(0, TEXT("RegsiterClass Failed"), TEXT("Error"), MB_OK);
		return 1;
	}

	HWND WindHandle = CreateWindowEx(0, klasa, app, WS_OVERLAPPEDWINDOW, 0, 0, X_RES, Y_RES, 0, 0, hInstance, 0);
	if(!WindHandle){
		MessageBox(0, TEXT("CreateWindow Failed"), TEXT("Error"), MB_OK);
		return 2;
	}

	ShowWindow(WindHandle, nShowCmd);
	UpdateWindow(WindHandle);

	//Ustawienie domy�lnych warto�ci dla timera.
	SetTimer(WindHandle, TIMER_ID, TIMER_DELAY, 0); 

	MSG msg;
	while(GetMessage(&msg, 0,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(klasa, hInstance);
	KillTimer(WindHandle, TIMER_ID); //usuni�cie zegara, �eby nie by�o potem wyciek�w.
	return msg.wParam;
}