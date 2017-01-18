#pragma once

#include "blocking_queue.h"

class IPCBuffer;

///
// 进程间通信，封装进程间通信操作
///
class IPC
{
public:
    ///
    // 委托类
    ///
    class Delegate
    {
    public:
        ///
        // 收取到了 IPC 消息
        ///
        virtual void OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data) {};
    };

private:
    ///
    // 封装要发送的信息
    ///
    struct MessageInfo
    {
        // 对端消息窗口
        HWND recv_window;

        // 消息
        unsigned int message;

        // 附加消息，仅在 IPC 类内部使用
        unsigned int internal_message;

        // 要发送的数据
        std::shared_ptr<IPCBuffer> data;
    };

private:
    IPC();
    ~IPC();

public:

    ///
    // 获取单例
    ///
    static IPC* GetInstance();

    ///
    // 初始化
    ///
    bool Initialize(const wchar_t* recv_window_class_name, const wchar_t* recv_window_name);

    ///
    // 反初始化
    ///
    void UnInitialize();

    ///
    // 设置委托
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // 向指定窗口投递一条消息
    ///
    bool PostIPCMessage(HWND recv_window, unsigned int message, std::shared_ptr<IPCBuffer> data);

    ///
    // 向指定窗口发送一条消息
    ///
    bool SendIPCMessage(
        HWND recv_window, 
        unsigned int message, 
        std::shared_ptr<IPCBuffer> data, 
        unsigned int time_out);

private:

    ///
    // 创建发送线程
    ///
    bool CreateSendThread();

    ///
    // 发送线程的过程函数
    ///
    static void SendThreadProc();

    ///
    // 创建接收窗口
    ///
    bool CreateRecvWindow(const wchar_t* recv_window_class_name, const wchar_t* recv_window_name);

    ///
    // 接收窗口的过程函数
    ///
    static LRESULT CALLBACK RecvWindowProc(
        HWND window_handle,
        UINT message,
        WPARAM w_param,
        LPARAM l_param);

    ///
    // 完成 IPC 消息的投递
    ///
    bool DoPostIPCMessage(
        HWND recv_window,
        unsigned int message,
        unsigned int internal_message,
        std::shared_ptr<IPCBuffer> data);

    ///
    // 完成 IPC 消息的发送
    ///
    static bool DoSendIPCMessage(HWND recv_window, std::shared_ptr<IPCBuffer> message_data);

    ///
    // 等待对端发来通知消息
    ///
    void WaitForNotifyMessage(
        HWND send_window, 
        unsigned int message, 
        unsigned int internal_message, 
        unsigned int time_out);

private:
    // 发送线程
    std::unique_ptr<std::thread> send_thread_;

    // 发送队列，存储了待发送的消息
    BlockingQueue<std::shared_ptr<MessageInfo>> send_queue_;

    // 接收窗口
    HWND recv_window_;

    // 委托处理器
    Delegate* delegate_;
};
