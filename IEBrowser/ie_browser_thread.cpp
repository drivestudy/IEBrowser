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
        // ���� IEBrowser ����
        browser_.reset(new IEBrowser());

        // ���������߳�
        unsigned int init_thread_id = ::GetCurrentThreadId();
        thread_.reset(new std::thread(BrowserThreadProc, shared_from_this(), setting, init_thread_id));

        // �ȴ��̳߳�ʼ�����
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

    // ֪ͨ ������߳�ִ�з���ʼ��
    unsigned int browser_thread_id = ::GetThreadId(thread_->native_handle());
    ::PostThreadMessage(browser_thread_id, WM_IEBROWSER_THREAD_UNINIT, 0, 0);

    // �ȴ�������̷߳���ʼ�����
    WaitForWindowMessage(WM_IEBROWSER_THREAD_UNINIT_FINISH);

    // �ȴ�������߳��˳�
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

    // ��ʼ�� IEBrowser ����
    bool init_succeed = self->browser_->Initialize(setting);

    // TODO : ���Դ��룬�ú�ɾ��
    self->browser_->Navigate(L"www.google.com");

    // �����̷߳���Ϣ����֪�߳��Ѿ���ʼ�����
    ::PostThreadMessage(init_thread_id, WM_IEBROWSER_THREAD_INIT_FINISH, init_succeed, 0);

    // ������Ϣѭ����ֱ���յ� WM_QUIT ��Ϣ
    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == WM_IEBROWSER_THREAD_UNINIT)
        {
            // ����ʼ�� IEBrowser ����
            self->browser_->UnInitialze();

            // �˳���Ϣѭ��
            ::PostQuitMessage(0);
        }
    }

    ::CoUninitialize();

    // ֪ͨ init �̣߳�������߳��Ѿ�����ʼ����ϣ������˳�
    ::PostThreadMessage(init_thread_id, WM_IEBROWSER_THREAD_UNINIT_FINISH, 0, 0);
}

void IEBrowserThread::WaitForWindowMessage(UINT message, WPARAM* w_param, LPARAM* l_param)
{
    MSG msg;

    // ȷ����Ϣ���д���
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
