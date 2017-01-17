#pragma once

#include "blocking_queue.h"
#include "ipc_buffer.h"

///
// 用于一对多的进程间通信，（一个 server 进程和多个 client 进程通信）
///
class IPCServer
{
public:
    ///
    // 委托类
    ///
    class Delegate
    {
    public:
        ///
        // 与某个 client 建立了连接
        ///
        virtual void OnClientConnected(unsigned int client_id) {};

        ///
        // 某个 client 与 server 断开了连接
        ///
        virtual void OnClientDisConnected(unsigned int client_id) {};

        ///
        // 收到来自 client 的命令
        ///
        virtual void OnReceiveClientCommand(
            unsigned int client_id,
            unsigned int command_id,
            std::shared_ptr<IPCBuffer> data) {};
    };

private:
    ///
    // 封装要发送的信息
    ///
    struct CommandInfo
    {
        unsigned int client_id;
        unsigned int command_id;
        HWND client_message_window;
        std::shared_ptr<IPCBuffer> data;
    };

private:
    IPCServer();
    ~IPCServer();

public:
    ///
    // 获取单例
    ///
    static IPCServer* GetInstance();

    ///
    // 启动 server, server_guid 在系统全局标识了这个 server
    ///
    bool Start(const wchar_t* server_guid);

    ///
    // 关闭 server, 必须在析构之前调用
    ///
    void Stop();

    ///
    // 设置委托
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // 移除委托
    ///
    void RemoveDelegate();

    ///
    // 向指定 client 发送一条命令
    ///
    bool SendCommand(
        unsigned int client_id,
        unsigned int command_id,
        std::shared_ptr<IPCBuffer> data,
        unsigned int time_out);

    ///
    // 向指定 client 投递一条命令
    ///
    bool PostCommand(
        unsigned int client_id,
        unsigned int command_id,
        std::shared_ptr<IPCBuffer> data);

private:
    // 以下是私有方法

    ///
    // 创建发送线程
    ///
    bool CreateSendThread();

    ///
    // 发送线程的过程函数
    ///
    static void SendThreadProc();

    ///
    // 完成发送操作
    ///
    static bool DoSendCommand(std::shared_ptr<CommandInfo> command_info);

    ///
    // 创建接收窗口
    ///
    bool CreateRecvWindow(const wchar_t* server_guid);

    ///
    // 接收窗口的过程函数
    ///
    static LRESULT CALLBACK RecvWindowProc(
        HWND window_handle,
        UINT message,
        WPARAM w_param,
        LPARAM l_param);

private:
    // 发送线程
    std::unique_ptr<std::thread> send_thread_;

    // 发送队列，存储了待发送的消息
    BlockingQueue<std::shared_ptr<CommandInfo>> send_queue_;

    // 接收窗口
    HWND recv_window_;

    // 委托处理器
    Delegate* delegate_;

    // 维护 client_id 和 消息窗口 关系的表，发送消息时，先通过 client_id 找到对端消息窗口，才能发送
    std::map<unsigned int, HWND> client_message_window_map_;
};
