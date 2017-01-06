#include "stdafx.h"

#include <crtdbg.h>

#include "ie_browser_manager.h"
#include "ie_browser_setting.h"
#include "win_utility.h"

CComModule _Module;

void Run(HINSTANCE instance, const wchar_t* command_line, int command_show)
{
    HWND frame_window = WinUtility::CreateFrameWindow();

    // ²âÊÔ´´½¨ä¯ÀÀÆ÷
    IEBrowserManager* browser_manager = IEBrowserManager::GetInstance();
    unsigned int browser_id;
    IEBrowserSetting setting;
    setting.parent_window_handle_ = frame_window;
    browser_manager->CreateBrowser(setting, browser_id);


    UINT_PTR timer_id = ::SetTimer(nullptr, 0, 5000, nullptr);

    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == WM_TIMER && msg.wParam == timer_id)
        {
            ::KillTimer(nullptr, timer_id);

            // ²âÊÔÏú»Ùä¯ÀÀÆ÷
            browser_manager->DestroyBrowser(browser_id);

            DestroyWindow(frame_window);

            ::PostQuitMessage(0);
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

    _Module.Init(NULL, instance);

    Run(instance, command_line, command_show);

    _Module.Term();

    return 0;
}