#include "stdafx.h"
#include "ie_browser_thread.h"

#include "ie_browser.h"
#include "ie_browser_window_message.h"

IEBrowserThread::IEBrowserThread()
{
}

IEBrowserThread::~IEBrowserThread()
{
}

bool IEBrowserThread::Initialize(IEBrowserSetting& setting)
{
    bool result = false;

    do
    {
        // 创建 IEBrowser 对象
        browser_.reset(new IEBrowser());

        // 创建工作线程
        unsigned int init_thread_id = ::GetCurrentThreadId();
        thread_.reset(new std::thread(BrowserThreadProc, shared_from_this(), setting, init_thread_id));

        // 等待线程初始化完成
        WPARAM init_succeed = false;
        WaitForWindowMessage(WM_IEBROWSER_THREAD_INIT_FINISH, &init_succeed);

        if (!init_succeed)
        {
            break;
        }

        result = true;

    } while (false);

    if (!result)
    {
        UnInitialize();
    }

    return result;
}

void IEBrowserThread::UnInitialize()
{
    assert(thread_ != nullptr && thread_->joinable());

    // 通知 浏览器线程执行反初始化
    unsigned int browser_thread_id = ::GetThreadId(thread_->native_handle());
    ::PostThreadMessage(browser_thread_id, WM_IEBROWSER_THREAD_UNINIT, 0, 0);

    // 等待浏览器线程反初始化完毕
    WaitForWindowMessage(WM_IEBROWSER_THREAD_UNINIT_FINISH);

    // 等待浏览器线程退出
    if (thread_->joinable())
    {
        thread_->join();
    }
    else
    {
        thread_->detach();
    }
}

void IEBrowserThread::BrowserThreadProc(
    std::shared_ptr<IEBrowserThread> self, 
    IEBrowserSetting setting, 
    unsigned int init_thread_id)
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // 初始化 IEBrowser 对象
    bool init_succeed = self->browser_->Initialize(setting);

    // TODO : 测试代码，用后删除
    self->browser_->Navigate(L"www.google.com");

    // 向主线程发消息，告知线程已经初始化完毕
    ::PostThreadMessage(init_thread_id, WM_IEBROWSER_THREAD_INIT_FINISH, init_succeed, 0);

    // 运行消息循环，直到收到 WM_QUIT 消息
    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == WM_IEBROWSER_THREAD_UNINIT)
        {
            // 反初始化 IEBrowser 对象
            self->browser_->UnInitialze();

            // 退出消息循环
            ::PostQuitMessage(0);
        }
    }

    ::CoUninitialize();

    // 通知 init 线程，浏览器线程已经反初始化完毕，即将退出
    ::PostThreadMessage(init_thread_id, WM_IEBROWSER_THREAD_UNINIT_FINISH, 0, 0);
}

void IEBrowserThread::WaitForWindowMessage(UINT message, WPARAM* w_param, LPARAM* l_param)
{
    MSG msg;

    // 确保消息队列存在
    // https://msdn.microsoft.com/en-us/library/ms644946(v=vs.85).aspx
    ::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == message)
        {
            if (w_param)
            {
                *w_param = msg.wParam;
            }
            if (l_param)
            {
                *l_param = msg.lParam;
            }

            break;
        }
    }
}
