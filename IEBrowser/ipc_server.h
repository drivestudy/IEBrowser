#pragma once

#include "blocking_queue.h"
#include "ipc_buffer.h"

///
// ����һ�Զ�Ľ��̼�ͨ�ţ���һ�� server ���̺Ͷ�� client ����ͨ�ţ�
///
class IPCServer
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
        // �յ����� client ������
        ///
        virtual void OnReceiveClientCommand(
            unsigned int client_id,
            unsigned int command_id,
            std::shared_ptr<IPCBuffer> data) {};
    };

private:
    ///
    // ��װҪ���͵���Ϣ
    ///
    struct CommandInfo
    {
        unsigned int client_id;
        unsigned int command_id;
        std::shared_ptr<IPCBuffer> data;
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
    // �Ƴ�ί��
    ///
    void RemoveDelegate();

    ///
    // ��ָ�� client ����һ������
    ///
    bool SendCommand(
        unsigned int client_id,
        unsigned int command_id,
        std::shared_ptr<IPCBuffer> data,
        unsigned int time_out);

    ///
    // ��ָ�� client Ͷ��һ������
    ///
    bool PostCommand(
        unsigned int client_id,
        unsigned int command_id,
        std::shared_ptr<IPCBuffer> data);

private:
    // ������˽�з���

    ///
    // ���������߳�
    ///
    bool CreateSendThread();

    ///
    // �����̵߳Ĺ��̺���
    ///
    static void SendThreadProc();

    ///
    // �������մ���
    ///
    bool CreateRecvWindow(const wchar_t* server_guid);

    ///
    // ���մ��ڵĹ��̺���
    ///
    static LRESULT CALLBACK RecvWindowProc(
        HWND window_handle,
        UINT message,
        WPARAM w_param,
        LPARAM l_param);

private:
    // �����߳�
    std::unique_ptr<std::thread> send_thread_;

    // ���Ͷ��У��洢�˴����͵���Ϣ
    BlockingQueue<CommandInfo> send_queue_;

    // ���մ���
    HWND recv_window_;

    // ί�д�����
    Delegate* delegate_;
};
