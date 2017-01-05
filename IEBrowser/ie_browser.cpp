#include "stdafx.h"
#include "ie_browser.h"

// WebBrowser �ؼ��� CLSID
const wchar_t* kWebBrowserCLSID = L"{8856F961-340A-11D0-A96B-00C04FD705A2}";

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
        // ������������
        if (!CreateBrowserWindow(parent_window_handle))
        {
            break;
        }

        // ���� WebBrowser �ؼ�
        if (!CreateWebBrowser())
        {
            break;
        }

        result = true;

    } while (false);

    if (!result)
    {
        UnInitialze();
    }

    return result;
}

void IEBrowser::UnInitialze()
{
    if (IsWindow())
    {
        if (web_browser_)
        {
            // ֹͣ���ڽ��еĵ���
            web_browser_->Stop();

            // ֪ͨ web_browser �ؼ��ر��Լ�
            web_browser_->ExecWB(OLECMDID_CLOSE, OLECMDEXECOPT_DONTPROMPTUSER, 0, 0);

            // �ͷ����ü���
            web_browser_ = nullptr;
        }

        DestroyWindow();
    }
}

bool IEBrowser::Navigate(const wchar_t * url)
{
    bool result = false;

    do
    {
        if (web_browser_ == nullptr)
        {
            break;
        }

        if (url == nullptr)
        {
            break;
        }

        CComVariant url_var(url);

        if (FAILED(web_browser_->Navigate2(&url_var, nullptr, nullptr, nullptr, nullptr)))
        {
            break;
        }

        result = true;

    } while (false);

    return result;
}

bool IEBrowser::Stop()
{
    bool result = false;

    do
    {
        if (web_browser_ == nullptr)
        {
            break;
        }

        web_browser_->Stop();

        result = true;

    } while (false);

    return result;
}

bool IEBrowser::Refresh()
{
    bool result = false;

    do
    {
        if (web_browser_ == nullptr)
        {
            break;
        }

        web_browser_->Refresh();

        result = true;

    } while (false);

    return result;
}

bool IEBrowser::GoForward()
{
    bool result = false;

    do
    {
        if (web_browser_ == nullptr)
        {
            break;
        }

        web_browser_->GoForward();

        result = true;

    } while (false);

    return result;
}

bool IEBrowser::GoBack()
{
    bool result = false;

    do
    {
        if (web_browser_ == nullptr)
        {
            break;
        }

        web_browser_->GoBack();

        result = true;

    } while (false);

    return result;
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

bool IEBrowser::CreateBrowserWindow(HWND parent_window_handle)
{
    bool result = false;

    do
    {
        DWORD style = 0;
        DWORD ex_style = 0;

        if (::IsWindow(parent_window_handle))
        {
            // ʹ���ߴ����˸�����, �򴴽��Ӵ���
            style = WS_CHILD | WS_CLIPSIBLINGS;
            ex_style = WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;
        }
        else
        {
            // ʹ����û�д��븸���ڣ��򴴽����㴰��
            parent_window_handle = nullptr;
            style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
            ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        }

        // ������������
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

bool IEBrowser::CreateWebBrowser()
{
    bool result = false;

    do
    {
        // ���� WebBrowser �ؼ�
        if (FAILED(CreateControl(kWebBrowserCLSID)))
        {
            break;
        }

        // ��ȡ IWebBrowser2 �ӿ�
        if (FAILED(QueryControl(IID_IWebBrowser2, (void**)&web_browser_)))
        {
            break;
        }

        result = true;

    } while (false);

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
