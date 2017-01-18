#pragma once

#include "blocking_queue.h"

class IPCBuffer;

///
// ���̼�ͨ�ţ���װ���̼�ͨ�Ų���
///
class IPC
{
public:
    ///
    // ί����
    ///
    class Delegate
    {
    public:
        ///
        // ��ȡ���� IPC ��Ϣ
        ///
        virtual void OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data) {};
    };

private:
    ///
    // ��װҪ���͵���Ϣ
    ///
    struct MessageInfo
    {
        // �Զ���Ϣ����
        HWND recv_window;

        // ��Ϣ
        unsigned int message;

        // ������Ϣ������ IPC ���ڲ�ʹ��
        unsigned int internal_message;

        // Ҫ���͵�����
        std::shared_ptr<IPCBuffer> data;
    };

private:
    IPC();
    ~IPC();

public:

    ///
    // ��ȡ����
    ///
    static IPC* GetInstance();

    ///
    // ��ʼ��
    ///
    bool Initialize(const wchar_t* recv_window_class_name, const wchar_t* recv_window_name);

    ///
    // ����ʼ��
    ///
    void UnInitialize();

    ///
    // ����ί��
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // ��ָ������Ͷ��һ����Ϣ
    ///
    bool PostIPCMessage(HWND recv_window, unsigned int message, std::shared_ptr<IPCBuffer> data);

    ///
    // ��ָ�����ڷ���һ����Ϣ
    ///
    bool SendIPCMessage(
        HWND recv_window, 
        unsigned int message, 
        std::shared_ptr<IPCBuffer> data, 
        unsigned int time_out);

private:

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
    bool CreateRecvWindow(const wchar_t* recv_window_class_name, const wchar_t* recv_window_name);

    ///
    // ���մ��ڵĹ��̺���
    ///
    static LRESULT CALLBACK RecvWindowProc(
        HWND window_handle,
        UINT message,
        WPARAM w_param,
        LPARAM l_param);

    ///
    // ��� IPC ��Ϣ��Ͷ��
    ///
    bool DoPostIPCMessage(
        HWND recv_window,
        unsigned int message,
        unsigned int internal_message,
        std::shared_ptr<IPCBuffer> data);

    ///
    // ��� IPC ��Ϣ�ķ���
    ///
    static bool DoSendIPCMessage(HWND recv_window, std::shared_ptr<IPCBuffer> message_data);

    ///
    // �ȴ��Զ˷���֪ͨ��Ϣ
    ///
    void WaitForNotifyMessage(
        HWND send_window, 
        unsigned int message, 
        unsigned int internal_message, 
        unsigned int time_out);

private:
    // �����߳�
    std::unique_ptr<std::thread> send_thread_;

    // ���Ͷ��У��洢�˴����͵���Ϣ
    BlockingQueue<std::shared_ptr<MessageInfo>> send_queue_;

    // ���մ���
    HWND recv_window_;

    // ί�д�����
    Delegate* delegate_;
};
