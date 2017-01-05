#include "stdafx.h"
#include "ie_browser.h"
#include "win_utility.h"

CComModule _Module;

void Run(HINSTANCE instance, const wchar_t* command_line, int command_show)
{
    HWND frame_window = WinUtility::CreateFrameWindow();

    // ���Դ��ڵĴ���
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

            // ���Դ��ڵ�����
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
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    _Module.Init(NULL, instance);

    Run(instance, command_line, command_show);

    _Module.Term();
    ::CoUninitialize();

    return 0;
}