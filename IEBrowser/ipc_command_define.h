#pragma once

enum IPCCommand
{
    // ֹͣ�����߳�
    CM_SEND_THREAD_QUIT,

    // ָ����������Ҫ�ظ�
    CM_EX_PLEASE_REPLY,
};

// IPC ģ��Ĵ�����Ϣ
enum IPCMessage
{
    // client ��������Ϣ����ʾĳ�������Ѿ��յ�
    WM_IPC_CLIENT_ROGER_THAT = WM_USER + 1,
};