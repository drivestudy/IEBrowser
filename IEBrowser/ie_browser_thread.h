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
    static void BrowserThreadProc(
        std::shared_ptr<IEBrowserThread> self, 
        IEBrowserSetting setting, 
        unsigned int init_thread_id);

    ///
    // �ȴ�ָ�� Window ��Ϣ
    // ͨ�� while(::GetMessage()) �ķ�ʽ���ȴ��������������÷�����Ϣѭ��
    ///
    void WaitForWindowMessage(UINT message, WPARAM* w_param = nullptr, LPARAM* l_param = nullptr);

private:

    // �̶߳���
    std::unique_ptr<std::thread> thread_;

    // �߳������е� IEBrowser ����
    std::unique_ptr<IEBrowser> browser_;
};
