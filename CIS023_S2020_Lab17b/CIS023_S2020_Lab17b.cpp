// Module:		CIS023_S2020_Lab17b.cpp
// Autor:		Miguel Antonio Logarta
// Date:		April 27, 2020
// Purpose:		Learn how to create a queue.
//				Initialize, load, access, and unload a stack.
//				Use the standard file openand standard file save dialog boxes.
//


#include "framework.h"
#include "CIS023_S2020_Lab17b.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hWnd_InFile;								// handle to file input edit box
HWND hWnd_btnOpen;								// butten, standard file open dialog
HWND hWnd_A;									// output labels
HWND hWnd_E;
HWND hWnd_I;
HWND hWnd_O;
HWND hWnd_U;

char fInputPath[MAX_PATH];						// input file name, selected by user

vector<char> vNode;								// Create queue

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool GetOpenFileName();							// prompt for read filespec using common open dialog box
bool LoadFile();								// load file contents into vector
void Count(HWND);								// cont characters in vector

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
    LoadStringW(hInstance, IDC_CIS023S2020LAB17B, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIS023S2020LAB17B));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIS023S2020LAB17B));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+0);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIS023S2020LAB17B);
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
      CW_USEDEFAULT, 0, 410, 200, nullptr, nullptr, hInstance, nullptr);

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
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BN_CLICKED:	// capture button click
			if (LOWORD(lParam) == (WORD)hWnd_btnOpen)	// launch standard file open dialog
			{
				if (GetOpenFileName())					// input filespec
				{
					if (LoadFile())						// load file into vector
						Count(hWnd);					// count the vowels
				}

			}
			break;

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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CREATE:
	{
		CreateWindow(TEXT("STATIC"),
			TEXT("Input file name:"),
			WS_VISIBLE | WS_CHILD, 20, 10, 400, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("A"),
			WS_VISIBLE | WS_CHILD, 150, 40, 10, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("E"),
			WS_VISIBLE | WS_CHILD, 200, 40, 10, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("I"),
			WS_VISIBLE | WS_CHILD, 250, 40, 10, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("O"),
			WS_VISIBLE | WS_CHILD, 300, 40, 10, 20, hWnd, NULL, NULL, NULL);

		CreateWindow(TEXT("STATIC"),
			TEXT("U"),
			WS_VISIBLE | WS_CHILD, 350, 40, 10, 20, hWnd, NULL, NULL, NULL);

		hWnd_InFile = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 150, 10, 250, 20, hWnd, NULL, NULL, NULL);

		hWnd_A = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 150, 65, 50, 20, hWnd, NULL, NULL, NULL);

		hWnd_E = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 200, 65, 50, 20, hWnd, NULL, NULL, NULL);

		hWnd_I = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 250, 65, 50, 20, hWnd, NULL, NULL, NULL);

		hWnd_O = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 300, 65, 50, 20, hWnd, NULL, NULL, NULL);

		hWnd_U = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_VISIBLE | WS_CHILD, 350, 65, 50, 20, hWnd, NULL, NULL, NULL);

		hWnd_btnOpen = CreateWindow(TEXT("BUTTON"),
			TEXT("Open"),
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			160, 100, 70, 30, hWnd, NULL, NULL, NULL);

		// Set the window label
		SetWindowText(hWnd, _T("CIS 023, Spring, 2020, Lab17b Miguel Antonio Logarta"));

	}
	break;

	case WM_DESTROY:
		vNode.clear();					// Clear the vector
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


// use common file open dialog box to get input filespec
bool GetOpenFileName()
{
	char szFileNameIN[MAX_PATH];

	// get the input file name
	OPENFILENAME ofn;
	ZeroMemory(&fInputPath, sizeof(fInputPath));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = LPWSTR("Any File\0*.*\0");
	ofn.lpstrFile = LPWSTR(fInputPath);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = LPWSTR(szFileNameIN);
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrTitle = LPWSTR("Select an input File");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(LPOPENFILENAMEA(&ofn))) // user selected an input file
	{
		// display input file name on window
		size_t pReturnValue;
		wchar_t wcstr[MAX_LOADSTRING];
		mbstowcs_s(&pReturnValue, wcstr, MAX_LOADSTRING, szFileNameIN, MAX_LOADSTRING);
		SendMessage(hWnd_InFile, WM_SETTEXT, NULL, LPARAM(wcstr));

		return true;
	}
	else
		return false;
}

// load file contents into vector
bool LoadFile()
{
	ifstream inFile;				// file handles
	char c;							// a character from the file

	try
	{
		// make sure the file will open before creating the stack
		inFile.open(fInputPath);
		if (inFile.bad())
			throw 1;

		while (inFile.good())			// read the entire file into the stack
		{
			inFile.get(c);
			vNode.push_back(c);			// Add the char to the queue
		}

		inFile.close();					// done with the input file

		return true;
	}
	catch (int)							// file open error
	{
		MessageBox(NULL,
			TEXT("Unable to open input file."),
			TEXT("File Open Error"),
			MB_ICONEXCLAMATION);

		return false;
	}

	return true;

}

// count character in the vector
void Count(HWND hWnd)
{
	char c = 0;
	int a = 0, e = 0, i = 0, o = 0, u = 0;
	TCHAR szOutput[TCHAR_SIZE];

	while (vNode.size() > 0)			// Keep looping until the queue is empty
	{
		// Count how many instances of each vowel exists in the file
		switch (toupper(vNode.at(0)))
		{
		case 'A':
			a += 1;
			break;
		case 'E':
			e += 1;
			break;
		case 'I':
			i += 1;
			break;
		case 'O':
			o += 1;
			break;
		case 'U':
			u += 1;
			break;
		default:
			break;
		}
		vNode.erase(vNode.begin());		// Delete the first item in the queue
	}



	// output
	InttoTCHAR(a, szOutput);
	SetWindowText(hWnd_A, szOutput);
	InttoTCHAR(e, szOutput);
	SetWindowText(hWnd_E, szOutput);
	InttoTCHAR(i, szOutput);
	SetWindowText(hWnd_I, szOutput);
	InttoTCHAR(o, szOutput);
	SetWindowText(hWnd_O, szOutput);
	InttoTCHAR(u, szOutput);
	SetWindowText(hWnd_U, szOutput);

}