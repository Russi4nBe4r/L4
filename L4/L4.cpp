// L4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "L4.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HDC hdc;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_L4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_L4));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_L4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_L4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

HWND static1;
POINT coords;
static HWND Static1;
static HWND Static2;
HANDLE hThread1;
HANDLE hThread2;
DWORD WINAPI Thread1(LPVOID th1);
DWORD WINAPI Thread2(LPVOID th2);
BOOL thread1_working = false;
BOOL thread2_working = false;
HWND hWnd;
LPARAM lParam;
int tSize1, tSize2;
static PAINTSTRUCT ps;

DWORD WINAPI Thread1(LPVOID th1)
{
    RECT rect3 = { 0, 0, 200 ,80 };
    wchar_t t1[20];
    wchar_t t2[20];
    int x, y;
    while (true)
    {
        GetCursorPos(&coords);
        x = coords.x;
        y = coords.y;

        tSize1 = wsprintf(t1, L"PosX: %d", x);
        tSize2 = wsprintf(t2, L"PosY: %d", y);
        hdc = GetDC(HWND(th1));
        SetTextColor(hdc, RGB(rand() % 155, rand() % 155, rand() % 155));
        TextOut(hdc, 20, 20, t1, tSize1);//xc);
        TextOut(hdc, 20, 40, t2, tSize2);
        ReleaseDC(HWND(th1), hdc);
        Sleep(200);
        InvalidateRect(HWND(th1), &rect3, TRUE);
        UpdateWindow(HWND(th1));
    }
    return 0;
}
DWORD WINAPI Thread2(LPVOID th2)
{
    RECT rect3 = { 110, 140, 460, 490 };
    RECT rectl = { 110, 140, 285, 490 };
    RECT rectr = { 285, 140, 460, 490 };
    InvalidateRect(HWND(th2), &rect3, TRUE);
    UpdateWindow(HWND(th2));
    Sleep(1000);
    while (true)
    {
        hdc = GetDC(HWND(th2));
        FillRect(hdc, &rectl, (HBRUSH)RGB(255, 255, 255));
        ReleaseDC(HWND(th2), hdc);
        Sleep(1000);
        InvalidateRect(HWND(th2), &rect3, TRUE);
        UpdateWindow(HWND(th2));
        hdc = GetDC(HWND(th2));
        FillRect(hdc, &rectr, (HBRUSH)RGB(255, 255, 255));
        ReleaseDC(HWND(th2), hdc);
        Sleep(1000);
        InvalidateRect(HWND(th2), &rect3, TRUE);
        UpdateWindow(HWND(th2));
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
                case IDC_CURSOR: //курсор
                {
                    if (!thread1_working)
                    {
                        hThread1 = CreateThread(NULL, 0, Thread1, hWnd, 0, NULL);
                    }
                    thread1_working = true;
                    break;

                }
                case IDC_CIRCLE: //круг
                {
                    if (!thread2_working)
                    {
                        hThread2 = CreateThread(NULL, 0, Thread2, hWnd, 0, NULL);
                    }
                    thread2_working = true;
                    break;
                }
                case IDC_STOP_CURSOR:
                {
                    if (hThread1)
                    {
                        TerminateThread(hThread1, 0);
                        CloseHandle(hThread1);
                    }
                    thread1_working = false;
                    break;
                }
                case IDC_STOP_CIRCLE:
                {
                    if (hThread1)
                    {
                        TerminateThread(hThread2, 0);
                        CloseHandle(hThread2);
                    }
                    thread2_working = false;
                    break;
                }
                case IDC_STOP_ALL:
                {
                    if (hThread1)
                    {
                        TerminateThread(hThread1, 0);
                        CloseHandle(hThread1);
                        thread1_working = false;
                    }

                    if (thread2_working)
                    {
                        TerminateThread(hThread2, 0);
                        CloseHandle(hThread2);
                        thread2_working = false;
                    }
                    break;
                }
            }
        }
        break;
    case WM_PAINT:
        hdc = GetDC(hWnd);
        hdc = BeginPaint(hWnd, &ps);
        if (thread2_working)
        {
            HBRUSH hbr = CreateSolidBrush(RGB(0, 0, 0));           //285;315
            HPEN hpn = CreatePen(PS_NULL, 0, RGB(255, 0, 0));
            int color_c = 0;
            SelectObject(hdc, hbr);
            SelectObject(hdc, hpn);
            for (int i = 150; i > 0; i = i - 10)
            {
                hbr = CreateSolidBrush(RGB(color_c, color_c, color_c));
                Ellipse(hdc, 285 - i, 315 - i, 285 + i, 315 + i);
                color_c = color_c + 15;
                SelectObject(hdc, hbr);
            }

        }

        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}