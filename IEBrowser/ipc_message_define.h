#pragma once

// IPC ģ��Ĵ�����Ϣ
enum IPCMessage
{
    // ֹͣ�����߳�
    WM_IPC_SEND_THREAD_QUIT = WM_USER + 1,

    // ָ����������Ҫ�ظ�
    WM_IPC_EX_PLEASE_REPLY,

    // ָ��������Ϣ�Ѿ��յ�
    WM_IPC_EX_ROGER_THAT,

    // client ���͵���������
    WM_IPC_CLIENT_CONNECT,

    // client ���͵ĶϿ���������
    WM_IPC_CLIENT_DISCONNECT,
};