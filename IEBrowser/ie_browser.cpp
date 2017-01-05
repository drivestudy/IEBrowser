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
        DWORD style = 0;
        DWORD ex_style = 0;

        if (::IsWindow(parent_window_handle))
        {
            // 使用者传入了父窗口, 则创建子窗口
            style = WS_CHILD | WS_CLIPSIBLINGS;
            ex_style = WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;
        }
        else
        {
            // 使用者没有传入父窗口，则创建顶层窗口
            parent_window_handle = nullptr;
            style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
            ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        }

        if (Create(
            parent_window_handle,
            CWindow::rcDefault,
            IEBROWSER_WINDOW_NAME,
            style,
            ex_style) == nullptr)
        {
            break;
        }

        UpdateVisible();
        UpdateSize();

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

void IEBrowser::UpdateVisible()
{
    CWindow parent_window = GetParent();
    if (parent_window.IsWindow())
    {
        ShowWindow(parent_window.IsWindowVisible() ? SW_SHOW : SW_HIDE);
    }
    else
    {
        ShowWindow(SW_SHOW);
        UpdateWindow();
    }
}

void IEBrowser::UpdateSize()
{
    CWindow parent_window = GetParent();
    if (parent_window.IsWindow())
    {
        RECT parent_window_rect;
        parent_window.GetClientRect(&parent_window_rect);
        
        MoveWindow(&parent_window_rect);
    }
}
