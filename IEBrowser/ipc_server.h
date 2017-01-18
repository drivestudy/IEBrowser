#pragma once

#include "ipc.h"

class IPCBuffer;

///
// ����һ�Զ�Ľ��̼�ͨ�ţ���һ�� server ���̺Ͷ�� client ����ͨ�ţ�
///
class IPCServer :
    public IPC::Delegate
{
public:
    ///
    // ί����
    ///
    class Delegate
    {
    public:
        ///
        // ��ĳ�� client ����������
        ///
        virtual void OnClientConnected(unsigned int client_id) {};

        ///
        // ĳ�� client �� server �Ͽ�������
        ///
        virtual void OnClientDisConnected(unsigned int client_id) {};

        ///
        // �յ���Ϣ
        ///
        virtual void OnRecvIPCMessage(
            unsigned int client_id,
            unsigned int message,
            std::shared_ptr<IPCBuffer> data) {};
    };

private:
    IPCServer();
    ~IPCServer();

public:
    ///
    // ��ȡ����
    ///
    static IPCServer* GetInstance();

    ///
    // ���� server, server_guid ��ϵͳȫ�ֱ�ʶ����� server
    ///
    bool Start(const wchar_t* server_guid);

    ///
    // �ر� server, ����������֮ǰ����
    ///
    void Stop();

    ///
    // ����ί��
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // ��ָ�� client ����һ������
    ///
    bool SendIPCMessage(
        unsigned int client_id,
        unsigned int message,
        std::shared_ptr<IPCBuffer> data,
        unsigned int time_out);

    ///
    // ��ָ�� client Ͷ��һ������
    ///
    bool PostIPCMessage(
        unsigned int client_id,
        unsigned int message,
        std::shared_ptr<IPCBuffer> data);

private:
    // ����ʵ�� IPC::Delegate �ӿڵķ���

    ///
    // �յ��˶Զ˷����� IPC ��Ϣ
    ///
    void OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data);

private:
    // ί�д�����
    Delegate* delegate_;

    // ά�� client_id �� ��Ϣ���� ��ϵ�ı�������Ϣʱ����ͨ�� client_id �ҵ��Զ���Ϣ���ڣ����ܷ���
    std::map<unsigned int, HWND> client_window_map_;
};
