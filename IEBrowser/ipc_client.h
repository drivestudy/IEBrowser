#pragma once

#include "ipc.h"

class IPCBuffer;

///
// ����һ�Զ�Ľ��̼�ͨ��, (һ�� server ���̺Ͷ�� client ����)
// ��װ client �����ϵ�ͨ�Žӿ�
///
class IPCClient :
    public IPC::Delegate
{
public:
    class Delegate
    {
    public:
        ///
        // �յ����� server ������
        ///
        virtual void OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data) {};
    };

private:
    IPCClient();
    ~IPCClient();

public:
    ///
    // ��ȡ����
    ///
    static IPCClient* GetInstance();

    ///
    // ���ӵ� server
    ///
    bool Connect(const wchar_t* server_guid);

    ///
    // �� server �Ͽ�����
    ///
    void DisConnect();

    ///
    // ����ί��
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // �� server Ͷ��һ������
    ///
    bool PostIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data);

    ///
    // �� server ����һ������
    ///
    bool SendIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data, unsigned int time_out);

private:
    ///
    // ���� server �Ľ��մ���
    ///
    HWND FindServerRecvWindow(const wchar_t* server_guid);

private:
    // ί�д�����
    Delegate* delegate_;

    // server �Ľ��մ���
    HWND server_recv_window_;
};
