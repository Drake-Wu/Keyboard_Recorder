// 键盘记录器.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "键盘记录器.h"
#include "iostream"
#define MAX_LOADSTRING 100
#define BUTTON_RECORD	0x0002
#define BUTTON_REPEAT	0x0003
#define RECORD_STOP	0x0004
#define REPEAT_STOP	0x0005
#define ID_TIMER_RECORD	0
#define ID_TIMER_REPEAT	1

#pragma warning(disable:4996)
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
static int keyboard_record_status = FALSE;
// 此代码模块中包含的函数的前向声明:
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

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
HWND hBtnRecord = NULL;
HWND hBtnRecordStop = NULL;
HWND hBtnRepeat = NULL;
HWND hBtnRepeatStop = NULL;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   hBtnRecord = CreateWindow(
	   TEXT("button"), //按钮控件的类名
	   TEXT("键盘记录"),
	   WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
	   30 /*X坐标*/, 20 /*Y坐标*/, 150 /*宽度*/, 50/*高度*/,
	   hWnd, (HMENU)BUTTON_RECORD /*控件唯一标识符*/, hInst, NULL
   );
   hBtnRecordStop = CreateWindow(
	   TEXT("button"), //按钮控件的类名
	   TEXT("STOP"),
	   WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
	   230 /*X坐标*/, 20 /*Y坐标*/, 150 /*宽度*/, 50/*高度*/,
	   hWnd, (HMENU)RECORD_STOP /*控件唯一标识符*/, hInst, NULL
   );
   hBtnRepeat = CreateWindow(
	   TEXT("button"), //按钮控件的类名
	   TEXT("重复记录"),
	   WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
	   30 /*X坐标*/, 100 /*Y坐标*/, 150 /*宽度*/, 50/*高度*/,
	   hWnd, (HMENU)BUTTON_REPEAT /*控件唯一标识符*/, hInst, NULL
   );
   hBtnRepeatStop = CreateWindow(
	   TEXT("button"), //按钮控件的类名
	   TEXT("STOP"),
	   WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
	   230 /*X坐标*/, 100 /*Y坐标*/, 150 /*宽度*/, 50/*高度*/,
	   hWnd, (HMENU)REPEAT_STOP /*控件唯一标识符*/, hInst, NULL
   );
   EnableWindow(hBtnRecordStop, FALSE);
   EnableWindow(hBtnRepeatStop, FALSE);
   if (!hWnd)
   {
      return FALSE;
   }
   //caps_status = GetKeyState(VK_CAPITAL) & 0x1;
   AllocConsole();
   freopen("CONOUT$", "w", stdout);
   freopen("CONOUT$", "w", stderr);
   freopen("CONIN$", "r", stdin);
   RAWINPUTDEVICE rid = { 0 };
   rid.usUsagePage = 0x01;
   rid.usUsage = 0x06; //keyboard
   rid.dwFlags = RIDEV_INPUTSINK;
   rid.hwndTarget = hWnd;

   RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

void KeyBoardRecorder(HRAWINPUT hRawInput)
{
	RAWINPUT input = { 0 };
	UINT size = sizeof(input);
	GetRawInputData(hRawInput, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER));
	if (input.data.keyboard.VKey <= '9' && input.data.keyboard.VKey >= '0' && input.data.keyboard.Flags == RI_KEY_MAKE)
		printf("%c", input.data.keyboard.VKey);
	else if (input.data.keyboard.VKey <= 'Z' && input.data.keyboard.VKey >= 'A' && input.data.keyboard.Flags == RI_KEY_MAKE)
	{
		if ((GetKeyState(VK_CAPITAL) & 0x1) ^ ((GetAsyncKeyState(VK_SHIFT) & 0x8000) >> 15))
			printf("%c", input.data.keyboard.VKey);
		else
			printf("%c", input.data.keyboard.VKey + ('a' - 'A'));
	}
	else if (input.data.keyboard.VKey <= VK_NUMPAD9 && input.data.keyboard.VKey >= VK_NUMPAD0 && input.data.keyboard.Flags == RI_KEY_MAKE)
		printf("%c", input.data.keyboard.VKey - (VK_NUMPAD0 - '0'));
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_TIMER:
		switch (wParam)
		{
		case ID_TIMER_RECORD:
			KillTimer(hWnd, ID_TIMER_RECORD);
			EnableWindow(hBtnRecordStop, TRUE);
			keyboard_record_status = TRUE;
			break;
		case ID_TIMER_REPEAT:
			break;
		}
		break;
	case WM_INPUT:
		{
			HRAWINPUT hRawInput = (HRAWINPUT)lParam;
			if(keyboard_record_status)
				KeyBoardRecorder(hRawInput);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case BUTTON_RECORD:
				SetTimer(hWnd, ID_TIMER_RECORD, 3000, 0);
				EnableWindow(hBtnRecord, FALSE);
				printf("keyboard\n");
				break;
			case RECORD_STOP:
				EnableWindow(hBtnRecord, TRUE);
				EnableWindow(hBtnRecordStop, FALSE);
				keyboard_record_status = FALSE;
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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
