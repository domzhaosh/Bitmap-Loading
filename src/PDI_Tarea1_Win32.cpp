// PDI_Tarea1_Win32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PDI_Tarea1_Win32.h"
#include "zmouse.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
LPWSTR windowTitle = L"Lector de Bitmaps";		// Titulo Ventana
BMPSYSTEMFILE_S BMPsys;							// Struct para el manejo de variables

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	FileInfo(HWND, UINT, WPARAM, LPARAM);
void DrawBitmap (HDC hDC, INT x, INT y, HBITMAP hBitmap, DWORD dwROP);
HBITMAP Rotate90(HDC hDC, HBITMAP hSourceBitmap);
FLOAT zoomValue = 1;
int srcWidth;
int srcHeight;
int destWidth;
int destHeight;
int ySrc;
int xSrc;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;
    BMPsys.noFileSelected = TRUE;
    BMPsys.bitmapFlipCase = 0;
    BMPsys.currentRotationValue = 0;
    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_PDI_TAREA1_WIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PDI_TAREA1_WIN32));

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
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
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PDI_TAREA1_WIN32));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= CreateSolidBrush(RGB(15, 15, 15)); // Original: (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PDI_TAREA1_WIN32);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassEx(&wcex);
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
    HWND hWnd;
    hInst = hInstance; // Store instance handle in our global variable
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;

    switch (message)
    {
        case WM_COMMAND:
            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;

                case ID_HELP_SHOWFILEINFO:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_FILEINFOBOX), hWnd, FileInfo);
                    break;

                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;

                case ID_EDITAR_NEGATIVO:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.bitmap->bitmapApplyNegative();
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_EDITAR_ESPEJOVERTICAL:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.bitmapFlipCase = BMPsys.bitmapFlipCase == -1 ? -2 : -1;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_EDITAR_ESPEJOHORIZONTAL:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.bitmapFlipCase = BMPsys.bitmapFlipCase == 1 ? 2 : 1;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_ROTAR_180:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.currentRotationValue += 180;
                        BMPsys.bitmapFlipCase = BMPsys.bitmapFlipCase == 180 ? 181 : 180;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_ROTAR_270C:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.currentRotationValue -= 270;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_ROTAR_90:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.currentRotationValue += 90;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_ROTAR_270:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.currentRotationValue += 270;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_ROTAR_90C:
                    if (BMPsys.noFileSelected)
                    {
                        MessageBox(hWnd, L"No has seleccionado ningun archivo", L"Error", MB_OK | MB_ICONEXCLAMATION);
                    }

                    else if (!BMPsys.noFileSelected)
                    {
                        BMPsys.currentRotationValue -= 90;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }

                    break;

                case ID_IMAGEN_ZOOMN:
                    if (zoomValue > 0.1)
                    {
                        zoomValue -= 0.1;
                    }

                    InvalidateRect(hWnd, NULL, TRUE);
                    break;

                case ID_IMAGEN_ZOOMP:
                    zoomValue += 0.1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;

                case ID_OPENFILE:
                    {
                        IFileOpenDialog * inputBitmap = NULL;
                        HRESULT hr;
                        COMDLG_FILTERSPEC aFileTypes[] = {{ L"Bitmap Files", L"*.bmp" }};
                        LPWSTR filename, filepath;
                        // Creacion del dialogo para seleccion de archivo
                        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&inputBitmap));
                        hr = inputBitmap->SetFileTypes(_countof(aFileTypes), aFileTypes);
                        hr = inputBitmap->SetTitle(L"Seleccione un archivo .bmp");
                        // Se muestra el dialogo
                        hr = inputBitmap->Show(NULL);
                        // Se obtienen los resultados de la seleccion
                        hr = inputBitmap->GetResult(&BMPsys.selectedFile);
                        zoomValue = 1;

                        if (hr == S_OK)
                        {
                            BMPsys.selectedFile->GetDisplayName(SIGDN_NORMALDISPLAY, &filename);
                            BMPsys.selectedFile->GetDisplayName(SIGDN_FILESYSPATH, &filepath);
                            BMPsys.noFileSelected = false;
                            SetWindowText(hWnd, filename);
                            InvalidateRect(hWnd, NULL, TRUE);
                            char char_filepath[1024];
                            size_t charactersConverted;
                            wcstombs_s(&charactersConverted, char_filepath, sizeof(char_filepath), filepath, _TRUNCATE);
                            //Leemos el archivo BMP y lo cargamos en memoria
                            BMPsys.bitmap = new Bitmap(char_filepath);
                            BMPsys.bitmapFlipCase = 0;
                            BMPsys.currentRotationValue = 0;
                            srcWidth = BMPsys.bitmap->getBitmapInfoHeader().biWidth;
                            srcHeight = BMPsys.bitmap->getBitmapInfoHeader().biHeight;
                            destWidth = BMPsys.bitmap->width;
                            destHeight = BMPsys.bitmap->height;
                            ySrc = 0;
                            xSrc = 0;
                            InvalidateRect(hWnd, NULL, TRUE);
                        }

                        break;
                    }

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }

            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(250, 250, 250));
            GetClientRect(hWnd, &rect);
            SetTextAlign(hdc, TA_CENTER);

            if (BMPsys.noFileSelected)
            {
                TextOut(hdc, (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2, L"Please open a .bmp file", 23);
            }

            else if (!BMPsys.noFileSelected)
            {
                BITMAPINFO * bi = (BITMAPINFO *)malloc(sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD) * 256);

                for (int i = 0; i < 256; i++)
                {
                    bi->bmiColors[i] = BMPsys.bitmap->getColours()[i];
                }

                bi->bmiHeader = BMPsys.bitmap->getBitmapInfoHeader();
                BYTE * pPixels = (BMPsys.bitmap->getBitmapData() + BMPsys.bitmap->getBitmapFileHeader().bfOffBits);
                BYTE * ppvBits;
                HBITMAP hbmp = CreateDIBSection(NULL, bi, DIB_RGB_COLORS, (void **) &ppvBits, NULL, 0);
                SetDIBits(NULL, hbmp, 0, BMPsys.bitmap->height, pPixels, bi, DIB_RGB_COLORS);
                DrawBitmap(hdc, (rect.right - rect.left) / 2 - BMPsys.bitmap->width * zoomValue / 2,
                           (rect.bottom - rect.top) / 2 - BMPsys.bitmap->height * zoomValue / 2, hbmp, SRCCOPY);
            }

            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

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

// Manejador para informacion de BMP
INT_PTR CALLBACK FileInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
        case WM_INITDIALOG:
            if (!BMPsys.noFileSelected)
            {
                SetDlgItemText(hDlg, IDC_FILEINFO_TEXT, BMPsys.bitmap->GeneralBitmapInfo());
                return (INT_PTR)TRUE;
            }

            else
            {
                SetDlgItemText(hDlg, IDC_FILEINFO_TEXT, L"No has seleccionado ningun archivo");
            }

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

void DrawBitmap (HDC hDC, INT x, INT y, HBITMAP hBitmap, DWORD dwROP)
{
    HBITMAP finalBitmap = hBitmap;
    INT positiveRotationValue = BMPsys.currentRotationValue > 0 ? BMPsys.currentRotationValue % 360 : (360 + BMPsys.currentRotationValue % 360) % 360;
    stringstream ss;

    if (positiveRotationValue == 90 || positiveRotationValue == 270)
    {
        INT clWndWidth = 2 * x + BMPsys.bitmap->width;
        INT clWndHeight = 2 * y + BMPsys.bitmap->height;
        x = (clWndWidth - BMPsys.bitmap->height) / 2;
        y = (clWndHeight - BMPsys.bitmap->width) / 2;
    }

    switch (positiveRotationValue)
    {
        case 90:
            finalBitmap = Rotate90(hDC, hBitmap);
            srcWidth = -BMPsys.bitmap->height;
            srcHeight = -BMPsys.bitmap->width;
            destWidth = BMPsys.bitmap->height;
            destHeight = BMPsys.bitmap->width;
            xSrc = BMPsys.bitmap->height;
            ySrc = BMPsys.bitmap->width;
            break;

        case 270:
            finalBitmap = Rotate90(hDC, hBitmap);

            if (BMPsys.bitmapFlipCase != 180)
            {
                srcWidth = BMPsys.bitmap->height;
                srcHeight = BMPsys.bitmap->width;
                destWidth = BMPsys.bitmap->height;
                destHeight = BMPsys.bitmap->width;
                xSrc = ySrc = 0;
            }

            break;

        case 180:
            if (BMPsys.bitmapFlipCase == 0 && BMPsys.bitmapFlipCase != -32)
            {
                BMPsys.bitmapFlipCase = 180;
                srcWidth = BMPsys.bitmap->width;
                srcHeight = BMPsys.bitmap->height;
                destWidth = BMPsys.bitmap->width;
                destHeight = BMPsys.bitmap->height;
            }

            if (BMPsys.bitmapFlipCase == -32 && BMPsys.bitmapFlipCase == 0)
            {
                BMPsys.bitmapFlipCase = 0;
            }

            if (BMPsys.bitmapFlipCase == -2)
            {
                srcHeight = -srcHeight;
                ySrc = ySrc == destHeight ? 0 : destHeight;
            }

            if (BMPsys.bitmapFlipCase == 2)
            {
                srcWidth = -srcWidth;
                xSrc = xSrc == destWidth ? 0 : destWidth;
            }

            break;

        case 0:
            positiveRotationValue = 360;
            BMPsys.currentRotationValue = 0;

            if (BMPsys.bitmapFlipCase != 180)
            {
                srcWidth = BMPsys.bitmap->width;
                srcHeight = BMPsys.bitmap->height;
                destWidth = BMPsys.bitmap->width;
                destHeight = BMPsys.bitmap->height;
                xSrc = ySrc = 0;
            }

            break;
    }

    switch (BMPsys.bitmapFlipCase)
    {
        case -1:
            srcHeight = -srcHeight;
            ySrc = ySrc == destHeight ? 0 : destHeight;
            break;

        case 1:
            srcWidth = -srcWidth;
            xSrc = xSrc == destWidth ? 0 : destWidth;
            break;

        case 180:
            srcHeight = -srcHeight;
            srcWidth = -srcWidth;
            ySrc = ySrc == destHeight ? 0 : destHeight;
            xSrc = xSrc == destWidth ? 0 : destWidth;
            BMPsys.bitmapFlipCase = -32;
            break;

        default:
            if (positiveRotationValue == 0 || positiveRotationValue == 360)
            {
                switch (BMPsys.bitmapFlipCase)
                {
                    case -2:
                        srcHeight = -srcHeight;
                        ySrc = ySrc == destHeight ? 0 : destHeight;
                        break;

                    case 2:
                        srcWidth = -srcWidth;
                        xSrc = xSrc == destWidth ? 0 : destWidth;
                        break;

                    case 181:
                        srcHeight = -srcHeight;
                        srcWidth = -srcWidth;
                        ySrc = ySrc == destHeight ? 0 : destHeight;
                        xSrc = xSrc == destWidth ? 0 : destWidth;
                        break;
                }
            }

            break;
    }

    HDC hDCBits = CreateCompatibleDC(hDC);
    SelectObject(hDCBits, finalBitmap);
    StretchBlt(hDC, x, y, destWidth * zoomValue, destHeight * zoomValue, hDCBits, xSrc, ySrc, srcWidth, srcHeight, SRCCOPY);
    DeleteDC(hDCBits);
}

HBITMAP Rotate90(HDC hDC, HBITMAP hSourceBitmap)
{
    HBITMAP hOldSourceBitmap, hOldDestBitmap, hDestBitmap;
    HDC hMemSrc, hMemDest;
    int height, width;
    int i, j;
    BITMAP iSrcBitmap;
    hMemSrc = CreateCompatibleDC(hDC);
    hMemDest = CreateCompatibleDC(hDC);
    GetObject(hSourceBitmap, sizeof(BITMAP), (LPSTR)&iSrcBitmap);
    width = iSrcBitmap.bmWidth;
    height = iSrcBitmap.bmHeight;
    hOldSourceBitmap = (HBITMAP)SelectObject(hMemSrc, hSourceBitmap);
    hDestBitmap = CreateCompatibleBitmap(hMemSrc, height, width);

    if (!hDestBitmap)
    {
        return(hDestBitmap);
    }

    hOldDestBitmap = (HBITMAP)SelectObject(hMemDest, hDestBitmap);

    for (i = 0; i < width; ++i)
    {
        for (j = 0; j < height; ++j)
        {
            SetPixel(hMemDest, j, width - 1 - i, GetPixel(hMemSrc, i, j));
        }
    }

    SelectObject(hMemSrc, hOldSourceBitmap);
    SelectObject(hMemDest, hOldDestBitmap);
    DeleteDC(hMemDest);
    DeleteDC(hMemSrc);
    return(hDestBitmap);
}
