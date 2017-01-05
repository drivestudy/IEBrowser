#include "stdafx.h"

#include <crtdbg.h>

#include "ie_browser.h"
#include "win_utility.h"

CComModule _Module;

void Run(HINSTANCE instance, const wchar_t* command_line, int command_show)
{
    HWND frame_window = WinUtility::CreateFrameWindow();

    // 测试窗口的创建
    IEBrowser ie_browser;
    ie_browser.Initialize(frame_window);

    ::SetTimer(frame_window, 0, 3000, nullptr);

    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.hwnd == frame_window && msg.message == WM_TIMER)
        {
            ::KillTimer(frame_window, 0);

            // 测试窗口的销毁
            ie_browser.UnInitialze();
            
            ::DestroyWindow(frame_window);

            break;
        }
    }
}

int APIENTRY wWinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    LPWSTR command_line,
    int command_show)
{
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    _Module.Init(NULL, instance);

    Run(instance, command_line, command_show);

    _Module.Term();
    ::CoUninitialize();

    return 0;
}