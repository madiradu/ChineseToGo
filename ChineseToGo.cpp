// ChineseToGo.cpp : Defines the entry point for the application.
//
#include <windows.h>
#include <gdiplus.h>
#include "framework.h"
#include "ChineseToGo.h"
#include <fstream>
#include <string>
#include <regex>
#include<vector>
#include<array>
#include<thread>

#include <locale>
#include<codecvt>
using namespace Gdiplus;


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

std::array<std::vector<std::array<std::wstring,3>>,10> txt;

static void initializare(int number){
    std::wstring line;
    std::wifstream infile("file"+std::to_string(number+1)+".html");
    infile.imbue(std::locale("zh_CN.UTF-8"));
    int i = 0;
    while (std::getline(infile,line))

    {
        if (!(line.length() > 29) || ((line.length() > 29) && ((unsigned int)line[28] < 0x4E00))) continue;
        std::array<std::wstring,3> temp;

        std::wstring delimiter = L"<span class='smmr'>";
        std::wstring token = line.substr((line.find(delimiter) == std::wstring::npos) ? (line.length() - 1) : line.find(delimiter)+ delimiter.length());
        delimiter = L"</span>";
        if (token != L"")
            temp[2] = token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter));

        temp[0] =  line[28];
        temp[1] = L"";

        txt[number].push_back(temp);

        if (txt[number][i][1] == (L"")) {
            std::wstring delimiter = L"style='color:#990000;'>";
            std::wstring token = line.substr((line.find(delimiter)== std::wstring::npos)?(line.length()-1): line.find(delimiter) + delimiter.length());
            delimiter = L"</span></td><td><span";
            if(token != L"")
            txt[number][i][1] = token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter));
        }
        if (txt[number][i][1] == (L"")) {
            std::wstring delimiter = L"style='color:#000099;'>";
            std::wstring token = line.substr((line.find(delimiter) == std::wstring::npos) ? (line.length() - 1) : line.find(delimiter) + delimiter.length());
            delimiter = L"</span></td><td><span";
            if (token != L"")
                txt[number][i][1] = token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter));
        }
        if (txt[number][i][1] == (L"")) {
            std::wstring delimiter = L"style='color:#cc7700;'>";
            std::wstring token = line.substr((line.find(delimiter) == std::wstring::npos) ? (line.length() - 1) : line.find(delimiter) + delimiter.length());
            delimiter = L"</span></td><td><span";
            if (token != L"")
                txt[number][i][1] = token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter));
        }
        if (txt[number][i][1] == (L"")) {
            std::wstring delimiter = L"style='color:#009900;'>";
            std::wstring token = line.substr((line.find(delimiter) == std::wstring::npos) ? (line.length() - 1) : line.find(delimiter) + delimiter.length());
            delimiter = L"</span></td><td><span";
            if (token != L"")
                txt[number][i][1] = token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter));
        }
        if (txt[number][i][1] == (L"")) {
            std::wstring delimiter = L"</a></td><td>";
            std::wstring token = line.substr((line.find(delimiter) == std::wstring::npos) ? (line.length() - 1) : line.find(delimiter) + delimiter.length());
            delimiter = L"</span></td><td><span";
            if (token != L"")
                txt[number][i][1] = token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter));
        }

        i++;
    } 
    infile.close();
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    for (int i = 0; i <  10; i++)
    {
        std::thread t(initializare,i);
        t.detach();
    }
    // TODO: Place code here.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHINESETOGO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHINESETOGO));

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
    Gdiplus::GdiplusShutdown(gdiplusToken);

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHINESETOGO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHINESETOGO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Rect destRect(200, 50, 150, 75);               
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    Graphics graphics(hdc);
    Pen pen(Color(255, 255, 0, 0), 2);
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
            case VK_RIGHT:
                InvalidateRect(hWnd, NULL, true);
                break;
            default:
                    break;
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        unsigned seed = time(0);
        srand(seed);
        int rndm = std::rand() % 10;
        int rndm1 = std::rand() % (txt[rndm].size() - 1);
        FontFamily   fontFamily(L"SimSun");
        Font         myFont(&fontFamily, 40, FontStyleBold, UnitPoint);  
        PointF origin(200.0f, 200.0f);
        SolidBrush blackBrush(Color(255, 255, 0, 0));
        std::wstring str= txt[rndm][rndm1][0]+ L" " + txt[rndm][rndm1][1] + L" " + txt[rndm][rndm1][2];
        StringFormat stringFormat;
        graphics.DrawString(str.c_str(), str.length(), &myFont, origin , &stringFormat, &blackBrush);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    EndPaint(hWnd, &ps);
    return 0;
}

// Message handler for about box.
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
