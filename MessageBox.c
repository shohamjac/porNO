#include <windows.h>
#include <windowsX.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "MessageBox.h"



LRESULT CALLBACK pfnCBTMsgBoxHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_CREATEWND)
	{
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP))
		{
			HWND hwnd = (HWND)wParam;

			// At this point you have the hwnd of the newly created 
			// message box that so you can position it at will
			MONITORINFO WindowMonitorInfo;
			WindowMonitorInfo.cbSize = sizeof(MONITORINFO);

			srand(time(NULL) + (int * ) hwnd);
			// randomize a position
			HMONITOR WindowMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
			BOOL success = GetMonitorInfoA(WindowMonitor, &WindowMonitorInfo);
			int PosX = WindowMonitorInfo.rcMonitor.left + rand() % (WindowMonitorInfo.rcMonitor.right - WindowMonitorInfo.rcMonitor.left + 1);
			int PosY = WindowMonitorInfo.rcMonitor.top + rand() % (WindowMonitorInfo.rcMonitor.bottom - WindowMonitorInfo.rcMonitor.top + 1);
			OutputDebugString("setting window position \n");

			HINSTANCE hInstance = GetModuleHandle(NULL);
			//SetWindowPos(hwnd, HWND_TOP, 100, 100, pcs->cx, pcs->cy, SWP_SHOWWINDOW); //this does not work becouse the window has not yet been created

			pcs->x = PosX;
			pcs->y = PosY;

		}
	}

	return (CallNextHookEx(NULL, nCode, wParam, lParam));
}


DWORD WINAPI JumpMessageBox(LPVOID lpParam)
{
	HHOOK hhookCBTProc = SetWindowsHookEx(WH_CBT,
		pfnCBTMsgBoxHook,
		0, GetCurrentThreadId());
	int SelectedButton = MessageBox(NULL, "got you", "You dirty boy!", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_SYSTEMMODAL);
	if (SelectedButton == IDRETRY) JumpMessageBox(NULL);
	
	//MessageBox(NULL, "You Naughty!", NULL, MB_OK | MB_ICONERROR);

	UnhookWindowsHookEx(hhookCBTProc);

	return 0;
}