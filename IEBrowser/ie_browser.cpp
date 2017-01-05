#include "stdafx.h"
#include "ie_browser.h"

IEBrowser::IEBrowser()
{
}

IEBrowser::~IEBrowser()
{
}

bool IEBrowser::Initialize(HWND parent_window_handle)
{
    bool result = false;

    do
    {
        if (!::IsWindow(parent_window_handle))
        {
            break;
        }

        if (Create(
            parent_window_handle,
            0,
            IEBROWSER_WINDOW_NAME,
            WS_CHILD | WS_CLIPSIBLINGS,
            WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE) == nullptr)
        {
            break;
        }

        result = true;

    } while (false);

    return result;
}

void IEBrowser::UnInitialze()
{
    if (IsWindow())
    {
        DestroyWindow();
    }
}

LRESULT IEBrowser::OnCreate(unsigned int message, WPARAM w_param, LPARAM l_param, BOOL & is_handled)
{
    LRESULT result = 0;

    result = DefWindowProc(message, w_param, l_param);
    is_handled = FALSE;

    return result;
}

LRESULT IEBrowser::OnDestroy(unsigned int message, WPARAM w_param, LPARAM l_param, BOOL & is_handled)
{
    LRESULT result = 0;

    result = DefWindowProc(message, w_param, l_param);
    is_handled = FALSE;

    return result;
}
