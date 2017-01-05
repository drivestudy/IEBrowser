/******************************************************************************
*
* @description: IE ������߳��࣬Ϊ IEBrowser �ṩ���л���
*               ÿ�� IEBrowser �����ڵ������߳���
*
* @author:      Dongyu
* @date:        2017.1.6
*
******************************************************************************/

#pragma once

#include "ie_browser_setting.h"

class IEBrowser;

///
// IE ������߳��࣬Ϊ IEBrowser �ṩ���л���
// IEBrowserThread �������һ���������̣߳�����������һ�� IEBrowser
// IEBrowserThread �а�����һ��ͨ���������ͨ���������ʹ���߷��������������������� IEBrowser
///
class IEBrowserThread :
    public std::enable_shared_from_this<IEBrowserThread>
{
private:
    ///
    // �̵߳�ǰ״̬
    ///
    enum ThreadState
    {
        // ��δ��ʼ��
        THREAD_STATE_UNINIT,

        // ��ʼ���ɹ�
        THREAD_STATE_INIT_SUCCEED,

        // ��ʼ��ʧ��
        THREAD_STATE_INIT_FAILED,
    };

public:
    IEBrowserThread();
    ~IEBrowserThread();

    ///
    // ��ʼ��
    ///
    bool Initialize(IEBrowserSetting& setting);

    ///
    // ����ʼ��
    ///
    void UnInitialize();

private:
    // ������˽�к���

    ///
    // �̺߳����������ڵ����߳���
    ///
    static void BrowserThreadProc(std::shared_ptr<IEBrowserThread> self, IEBrowserSetting setting);

    ///
    // ��ʼ�������̣߳������ڹ����߳������
    ///
    bool InitBrowserThread(IEBrowserSetting& setting);

private:
    // �̶߳���
    std::unique_ptr<std::thread> thread_;

    // �̵߳�ǰ״̬
    ThreadState thread_state_;

    // �����������ڱ��� thread_state_ ��Ա����
    std::mutex thread_state_mutex_;

    // �������������ڵȴ� thread_state_ ��״̬�л�
    std::condition_variable thread_state_condition_;

    // �߳������е� IEBrowser ����
    std::unique_ptr<IEBrowser> browser_;
};
