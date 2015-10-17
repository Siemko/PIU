	

    #include <windows.h>
    #include <time.h>
    #include <windowsx.h>
    #include <vector>
     
    using std::vector;
     
    #define ID_TIMER 1
     
    TCHAR NazwaAplikacji[] = TEXT("Aplikacja studencka");
    TCHAR NazwaKlasy[] = TEXT("OKNOGLOWNE");
     
    HBRUSH tlo=CreateSolidBrush(RGB(0,0,0));
    HBRUSH fioletowy,pomaranczowy;
     
    POINT paletka={0,20};
     
    PAINTSTRUCT ps;
    RECT rect;
     
    int licznik=0;
     
    TCHAR nazwa[10];
     
    bool pocisk=false;
    bool strzal=false;
    bool odstrzal=false;
     
    bool lewo = false;
    bool prawo = false;
     
    int x1 = 0;
     
    vector <POINT> wrogowie;
    vector <POINT> kulka;
     
    static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam)
    {
            switch (Komunikat)
            {
            case WM_CREATE:
                    {
                            SetTimer(Okno,ID_TIMER,100,NULL);
     
                            for(int i = 0; i<40; i++)
                            {
                                    POINT c = {5+i, 5};
                                    wrogowie.push_back(c);
                            }
     
                            InvalidateRect(Okno,NULL,true);
                    }
     
            case WM_TIMER:
                    {
                            switch(wParam)
                            {
                            case ID_TIMER:
                                    {
     
                                            if(strzal)
                                            {
                                                            kulka[0].y-=1;
     
                                                            if(kulka[0].y < 1)
                                                            {
                                                                    strzal = false;
                                                                    odstrzal = true;
                                                                    kulka[0].x-=2;
                                                            }
     
                                                            if(kulka[0].x < 1)
                                                            {
                                                                    prawo = true;
                                                                    lewo = false;
                                                            }
     
                                                            if(kulka[0].x > 50)
                                                            {
                                                                    lewo = true;
                                                                    prawo = false;
                                                            }
                                                   
                                                    for(int i = 0; i<wrogowie.size(); i++)
                                                    {
                                                            if(kulka[0].y == wrogowie[i].y && kulka[0].x == wrogowie[i].x)
                                                                    {
                                                                            wrogowie[i].y = -50;
                                                                            odstrzal = true;
                                                                            strzal = false;
                                                                            prawo = true;
                                                                            kulka[0].y+=1;
                                                                            licznik++;
                                                                    }
                                                    }
                                            }
                                            if(odstrzal)
                                            {
                                                    kulka[0].y+=1;
     
                                                            if(kulka[0].x < 1)
                                                            {
                                                                    prawo = true;
                                                                    lewo = false;
                                                            }
     
                                                            if(kulka[0].x > 50)
                                                            {
                                                                    lewo = true;
                                                                    prawo = false;
                                                            }
     
                                                    if((kulka[0].y >= 45 && kulka[0].y <= 48) && (kulka[0].x >= paletka.x-10 && kulka[0].x <= paletka.x+10))
                                                    {
                                                            strzal = true;
                                                            odstrzal = false;
                                                            prawo = false;
                                                            lewo = false;
                                                    }
                                            }
     
                                            if(prawo)
                                            {
                                                    kulka[0].x+=2;
                                            }
     
                                            if(lewo)
                                            {
                                                    kulka[0].x-=2;
                                            }
                                    }
                                    InvalidateRect(Okno,NULL,true);
                            }break;
                    }break;
     
            case WM_MOUSEMOVE:
                    {
                            paletka.x = (GET_X_LPARAM(lParam))/10;
                    }break;
     
            case WM_LBUTTONDOWN:
                    {
                            pocisk=true;
                            POINT c = {((GET_X_LPARAM(lParam))/10)-4,45};
                            kulka.push_back(c);
                    }break;
     
            case WM_LBUTTONUP:
                    {
                            strzal=true;
                    }break;
     
            case WM_PAINT:
                    {
                            HDC hdc=BeginPaint(Okno,&ps);
     
                            GetClientRect(Okno,&rect);
                            SetMapMode(hdc,MM_ISOTROPIC);
                            SetWindowExtEx(hdc,50,50,NULL);
                            SetViewportExtEx(hdc,rect.right,rect.bottom,NULL);
     
                            wsprintf(nazwa,TEXT("Punkty: %d"),licznik);
                            SetWindowText(Okno,nazwa);
     
                            pomaranczowy=CreateSolidBrush(RGB(255,100,55));
                            fioletowy=CreateSolidBrush(RGB(255,0,156));
     
                            SelectObject(hdc,pomaranczowy);
     
                            if(paletka.x >= rect.left && paletka.x <= rect.right)
                            {
                                    Rectangle(hdc,paletka.x,45,paletka.x-10,47);
                            }
     
                            SelectObject(hdc,fioletowy);
     
                            if(pocisk)
                            {
                                            Ellipse(hdc,kulka[0].x,kulka[0].y,kulka[0].x-1,kulka[0].y-1);
                            }
     
                            HBRUSH bialy=CreateSolidBrush(RGB(255,255,255));
     
                            SelectObject(hdc,bialy);
     
                            for(int i = 0; i<wrogowie.size(); i++)
                            {
                                    Rectangle(hdc, wrogowie[i].x, wrogowie[i].y, wrogowie[i].x+1, wrogowie[i].y+1);
                            }
     
                            EndPaint(Okno,&ps);
                    }break;
     
            case WM_DESTROY:
                    PostQuitMessage(0);
                    break;
            default:
                    return DefWindowProc(Okno, Komunikat, wParam, lParam);
            }
            return 0;
    }
     
    static bool RejestrujKlasy()
    {
            WNDCLASSEX wc;
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.cbClsExtra = wc.cbWndExtra = 0;
            wc.hbrBackground = tlo;
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpfnWndProc = &FunkcjaOkienkowa;
            wc.lpszClassName = NazwaKlasy;
            wc.lpszMenuName = NULL;
            wc.style = CS_HREDRAW | CS_VREDRAW;
            return (RegisterClassEx(&wc) != 0);
    }
     
    static void WyrejestrujKlasy()
    {
            UnregisterClass(NazwaKlasy, GetModuleHandle(NULL));
    }
     
    int WINAPI WinMain(HINSTANCE Instancja, HINSTANCE Poprzednia, LPSTR Parametry, int Widocznosc)
    {
            srand(time(NULL));
            // Zarejestruj klasê. Protestuj, je¿eli wyst¹pi³ b³¹d.
            if (!RejestrujKlasy()) {
                    MessageBox(NULL, TEXT("Nie uda³o siê zarejestrowaæ klasy okna!"),
                            NazwaAplikacji, MB_ICONSTOP | MB_OK);
                    return 1;
            }
            // Stwórz g³ówne okno. Równie¿ protestuj, je¿eli wyst¹pi³ b³¹d.
            HWND GlowneOkno = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE,
                    NazwaKlasy, TEXT("Okno"), WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
                    NULL, NULL, Instancja, NULL);
            if (GlowneOkno == NULL) {
                    MessageBox(NULL, TEXT("Nie uda³o siê stworzyæ g³ównego okna!"),
                            NazwaAplikacji, MB_ICONSTOP | MB_OK);
                    return 2;
            }
            // Wyœwietl i uaktualnij nowo stworzone okno.
            ShowWindow(GlowneOkno, Widocznosc);
            UpdateWindow(GlowneOkno);
            // G³ówna pêtla komunikatów w¹tku.
            MSG Komunikat;
            while (GetMessage(&Komunikat, NULL, 0, 0) > 0) {
                    TranslateMessage(&Komunikat);
                    DispatchMessage(&Komunikat);
            }
            // Zwolnij pamiêæ klas i zakoñcz proces.
            WyrejestrujKlasy();
            return static_cast<int>(Komunikat.wParam);
    }

