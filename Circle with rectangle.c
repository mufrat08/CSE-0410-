#include <windows.h>

#include <cmath>

#define ROUND(a) ((int) (a + 0.5))

/* set window handle */

static HWND sHwnd;

static COLORREF redColor=RGB(255,0,0);

static COLORREF blueColor=RGB(0,0,255);

static COLORREF greenColor=RGB(0,255,0);


void SetWindowHandle(HWND hwnd) {

    sHwnd=hwnd;

}


/* SetPixel */

void setPixel(int x,int y,COLORREF& color=redColor) {

    if(sHwnd==NULL) {

        MessageBox(NULL,"sHwnd was not initialized !","Error",MB_OK|MB_ICONERROR);

        exit(0);

    }

    HDC hdc=GetDC(sHwnd);

    SetPixel(hdc,x,y,color);

    ReleaseDC(sHwnd,hdc);

    return;

}


void circlePoints(int xCenter, int yCenter, int x, int y) {

    setPixel(xCenter + x, yCenter + y);

    setPixel(xCenter - x, yCenter + y);

    setPixel(xCenter + x, yCenter - y);

    setPixel(xCenter - x, yCenter - y);

    setPixel(xCenter + y, yCenter + x);

    setPixel(xCenter - y, yCenter + x);

    setPixel(xCenter + y, yCenter - x);

    setPixel(xCenter - y, yCenter - x);


}

void drawCircle(int xCenter, int yCenter, int radius) {

    int x = 0;

    int y = radius;

    int p = 1 - radius;

    circlePoints(xCenter, yCenter, x, y);

    while(x < y) {

        x++;

        if (p < 0) {

            p += 2 * x + 1;

        } else {

            y--;

            p += 2 * (x - y) + 1;

        }

        circlePoints(xCenter, yCenter, x, y);

    }

}

///////////////////////////////////////////////////////////////////
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y) {

    setPixel(xCenter + x, yCenter + y);

    setPixel(xCenter - x, yCenter + y);

    setPixel(xCenter + x, yCenter - y);

    setPixel(xCenter - x, yCenter - y);

}

void drawEllipse(int xCenter, int yCenter, int Rx, int Ry) {

    int Rx2 = Rx*Rx;

    int Ry2 = Ry*Ry;

    int twoRx2 = 2 * Rx2;

    int twoRy2 = 2 * Ry2;

    int p;

    int x = 0;

    int y = Ry;

    int px = 0;

    int py = twoRx2 * y;


    ellipsePlotPoints(xCenter, yCenter, x, y);


    /* For Region 1 */

    p = ROUND(Ry2 - (Rx2*Ry) + (0.25) * Rx2);

    while(px < py) {

        x++;

        px += twoRy2;

        if(p < 0) {

            p += Ry2 + px;

        } else {

            y--;

            py -= twoRx2;

            p += Ry2 + px - py;

        }

        ellipsePlotPoints(xCenter, yCenter, x, y);

    }


    /* For Region 2*/

    p = ROUND(Ry2 * (x + 0.5)*(x + 0.5) + Rx2 * (y - 1)*(y - 1) - Rx2 * Ry2);

    while(y > 0) {

        y--;

        py -= twoRx2;

        if(p > 0) {

            p += Rx2 - py;

        } else {

            x++;

            px += twoRy2;

            p += Rx2 - py + px;

        }

        ellipsePlotPoints(xCenter, yCenter, x, y);
    }
}
////////////////////////////////////////////////////////////////


void drawLineDDA(int xa, int ya, int xb, int yb) {

    int dx = xb - xa, dy = yb - ya, steps, k;

    float xIncrement, yIncrement, x = xa, y = ya;

    if(abs(dx) > abs(dy))
        steps = abs(dx);

    else
        steps = abs(dy);

    xIncrement = dx / (float) steps;

    yIncrement = dy / (float) steps;

    setPixel(ROUND(x), ROUND(y));

    for(int k = 0; k < steps; k++) {

        x += xIncrement;

        y += yIncrement;

        setPixel(x, y);

    }

}
///////////////////////////////////////////////////////////////////

void ractangle(int x, int y, int xx, int yy) {
    drawLineDDA(x,y, x+xx, y);
    drawLineDDA(x+xx,y, x+xx, y+yy);
    drawLineDDA(x,y+yy, x+xx, y+yy);
    drawLineDDA(x,y+yy, x, y);
}


/* Window Procedure WndProc */

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam) {

    switch(message) {

    case WM_PAINT:

        SetWindowHandle(hwnd);

        ractangle(500,300,400,300);
        drawEllipse(500+200,300+150,200,150);
        drawEllipse(500+200,300+150,50,150);

        break;

    case WM_CLOSE: // FAIL THROUGH to call DefWindowProc

        break;

    case WM_DESTROY:

        PostQuitMessage(0);

        return 0;

    default:

        break; // FAIL to call DefWindowProc //

    }

    return DefWindowProc(hwnd,message,wParam,lParam);

}



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow) {

    static TCHAR szAppName[] = TEXT("Circle");

    WNDCLASS wndclass;

    wndclass.style         = CS_HREDRAW|CS_VREDRAW ;

    wndclass.lpfnWndProc   = WndProc ;

    wndclass.cbClsExtra    = 0 ;

    wndclass.cbWndExtra    = 0 ;

    wndclass.hInstance     = hInstance ;

    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;

    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;

    wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;

    wndclass.lpszMenuName  = NULL ;

    wndclass.lpszClassName = szAppName ;


    // Register the window //

    if(!RegisterClass(&wndclass)) {

        MessageBox(NULL,"Registering the class failled","Error",MB_OK|MB_ICONERROR);

        exit(0);

    }


    // CreateWindow //

    HWND hwnd=CreateWindow(szAppName,"Mid Point Circle Drawing - Programming Techniques",

                           WS_OVERLAPPEDWINDOW,

                           CW_USEDEFAULT,

                           CW_USEDEFAULT,

                           CW_USEDEFAULT,

                           CW_USEDEFAULT,

                           NULL,

                           NULL,

                           hInstance,

                           NULL);

    if(!hwnd) {

        MessageBox(NULL,"Window Creation Failed!","Error",MB_OK);

        exit(0);

    }

    // ShowWindow and UpdateWindow //

    ShowWindow(hwnd,iCmdShow);

    UpdateWindow(hwnd);


    // Message Loop //

    MSG msg;

    while(GetMessage(&msg,NULL,0,0)) {

        TranslateMessage(&msg);

        DispatchMessage(&msg);

    }


    /* return no error to the operating system */

    return 0;

}
