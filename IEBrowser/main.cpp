#include "stdafx.h"

#include <crtdbg.h>

#include "ie_browser_thread.h"
#include "win_utility.h"

CComModule _Module;

void Run(HINSTANCE instance, const wchar_t* command_line, int command_show)
{
    // ≤‚ ‘¥¥Ω® IEBrowserThread
    std::shared_ptr<IEBrowserThread> browser_thread(new IEBrowserThread);

    IEBrowserSetting setting;
    browser_thread->Initialize(setting);

    UINT_PTR timer_id = ::SetTimer(nullptr, 0, 5000, nullptr);

    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == WM_TIMER && msg.wParam == timer_id)
        {
            ::KillTimer(nullptr, timer_id);

            // ≤‚ ‘œ˙ªŸ IEBrowserThread
            browser_thread->UnInitialize();

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

    _Module.Init(NULL, instance);

    Run(instance, command_line, command_show);

    _Module.Term();

    return 0;
}