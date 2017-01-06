#include "stdafx.h"
#include "ie_browser_thread.h"

#include "ie_browser.h"

IEBrowserThread::IEBrowserThread() :
    thread_state_(THREAD_STATE_UNINIT)
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
        thread_.reset(new std::thread(BrowserThreadProc, shared_from_this(), setting));

        // 等待线程初始化完成
        {
            std::unique_lock<std::mutex> lock_guard(thread_state_mutex_);

            while (thread_state_ == THREAD_STATE_UNINIT)
            {
                thread_state_condition_.wait(lock_guard);
            }
        }

        // 初始化失败，退出流程
        if (thread_state_ == THREAD_STATE_INIT_FAILED)
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
    assert(thread_ && thread_->joinable());

    if (thread_ && thread_->joinable())
    {
        HANDLE thread_handle = thread_->native_handle();
        assert(thread_handle != INVALID_HANDLE_VALUE && thread_handle != 0);

        DWORD thread_id = ::GetThreadId(thread_handle);
        assert(thread_id != 0);

        // 向线程发送退出命令
        if (::PostThreadMessage(thread_id, WM_QUIT, 0, 0) != 0)
        {
            // 等待线程退出
            thread_->join();
        }
    }
}

void IEBrowserThread::BrowserThreadProc(std::shared_ptr<IEBrowserThread> self, IEBrowserSetting setting)
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // 初始化工作线程
    bool is_succeed = self->InitBrowserThread(setting);

    {
        std::lock_guard<std::mutex> lock_guard(self->thread_state_mutex_);

        // 更新线程状态
        self->thread_state_ = is_succeed ? THREAD_STATE_INIT_SUCCEED : THREAD_STATE_INIT_FAILED;

        // 通知线程状态已经改变
        self->thread_state_condition_.notify_one();
    }

    // 运行消息循环，直到收到 WM_QUIT 消息
    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    // 反初始化 IEBrowser 对象
    self->browser_->UnInitialze();

    ::CoUninitialize();
}

bool IEBrowserThread::InitBrowserThread(IEBrowserSetting& setting)
{
    bool result = false;

    do
    {
        if (!browser_->Initialize(setting))
        {
            break;
        }

        // TODO : 测试代码，用后删除
        browser_->Navigate(L"www.google.com");

        result = true;

    } while (false);

    return result;
}
