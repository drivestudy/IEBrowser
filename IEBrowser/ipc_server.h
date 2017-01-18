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
        // �յ����� client ����Ϣ
        ///
        virtual void OnReceiveClientMessage(
            unsigned int client_id,
            unsigned int message,
            std::shared_ptr<IPCBuffer> data) {};
    };

private:
    ///
    // ��װҪ���͵���Ϣ
    ///
    struct MessageInfo
    {
        // �Զ� id
        unsigned int client_id;

        // ��Ϣ
        unsigned int message;

        // �Զ���Ϣ����
        HWND client_window;

        // Ҫ���͵�����
        std::shared_ptr<IPCBuffer> data;

        // ������Ϣ
        unsigned int ex_message;
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
    // ������˽�з���

    ///
    // ���Ͷ�ݲ���
    ///
    bool DoPostIPCMessage(
        unsigned int client_id,
        unsigned int message,
        std::shared_ptr<IPCBuffer> data,
        unsigned int ex_message);

    ///
    // �ȴ��Զ˷���ĳ����Ϣ
    ///
    bool WaitForClientMessage(
        unsigned int client_id, 
        unsigned int message, 
        unsigned int ex_message,
        unsigned int time_out);

    ///
    // ���������߳�
    ///
    bool CreateSendThread();

    ///
    // �����̵߳Ĺ��̺���
    ///
    static void SendThreadProc();

    ///
    // ��ɷ��Ͳ���
    ///
    static bool DoSendIPCMessage(std::shared_ptr<MessageInfo> message_info);

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
    BlockingQueue<std::shared_ptr<MessageInfo>> send_queue_;

    // ���մ���
    HWND recv_window_;

    // ί�д�����
    Delegate* delegate_;

    // ά�� client_id �� ��Ϣ���� ��ϵ�ı�������Ϣʱ����ͨ�� client_id �ҵ��Զ���Ϣ���ڣ����ܷ���
    std::map<unsigned int, HWND> client_window_map_;
};
