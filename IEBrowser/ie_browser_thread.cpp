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
        // ���� IEBrowser ����
        browser_.reset(new IEBrowser());

        // ���������߳�
        thread_.reset(new std::thread(BrowserThreadProc, shared_from_this(), setting));

        // �ȴ��̳߳�ʼ�����
        {
            std::unique_lock<std::mutex> lock_guard(thread_state_mutex_);

            while (thread_state_ == THREAD_STATE_UNINIT)
            {
                thread_state_condition_.wait(lock_guard);
            }
        }

        // ��ʼ��ʧ�ܣ��˳�����
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

        // ���̷߳����˳�����
        if (::PostThreadMessage(thread_id, WM_QUIT, 0, 0) != 0)
        {
            // �ȴ��߳��˳�
            thread_->join();
        }
    }
}

void IEBrowserThread::BrowserThreadProc(std::shared_ptr<IEBrowserThread> self, IEBrowserSetting setting)
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // ��ʼ�������߳�
    bool is_succeed = self->InitBrowserThread(setting);

    {
        std::lock_guard<std::mutex> lock_guard(self->thread_state_mutex_);

        // �����߳�״̬
        self->thread_state_ = is_succeed ? THREAD_STATE_INIT_SUCCEED : THREAD_STATE_INIT_FAILED;

        // ֪ͨ�߳�״̬�Ѿ��ı�
        self->thread_state_condition_.notify_one();
    }

    // ������Ϣѭ����ֱ���յ� WM_QUIT ��Ϣ
    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    // ����ʼ�� IEBrowser ����
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

        // TODO : ���Դ��룬�ú�ɾ��
        browser_->Navigate(L"www.google.com");

        result = true;

    } while (false);

    return result;
}
