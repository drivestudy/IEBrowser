#pragma once

enum IEBrowserWindowMessage
{
    // ������̳߳�ʼ����Ϻ󣬽����������Ϣ
    WM_IEBROWSER_THREAD_INIT_FINISH = WM_USER + 1,

    // Ҫ����ʼ�� ������߳�ʱ����Ҫ��������̷߳��������Ϣ
    WM_IEBROWSER_THREAD_UNINIT,

    // ������߳��Ѿ�����ʼ�����
    WM_IEBROWSER_THREAD_UNINIT_FINISH,
};