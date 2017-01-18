#pragma once

#include "ipc.h"

class IPCBuffer;

///
// 用于一对多的进程间通信, (一个 server 进程和多个 client 进程)
// 封装 client 进程上的通信接口
///
class IPCClient :
    public IPC::Delegate
{
public:
    class Delegate
    {
    public:
        ///
        // 收到来自 server 的命令
        ///
        virtual void OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data) {};
    };

private:
    IPCClient();
    ~IPCClient();

public:
    ///
    // 获取单例
    ///
    static IPCClient* GetInstance();

    ///
    // 连接到 server
    ///
    bool Connect(const wchar_t* server_guid);

    ///
    // 与 server 断开连接
    ///
    void DisConnect();

    ///
    // 设置委托
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // 向 server 投递一条命令
    ///
    bool PostIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data);

    ///
    // 向 server 发送一条命令
    ///
    bool SendIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data, unsigned int time_out);

private:
    ///
    // 查找 server 的接收窗口
    ///
    HWND FindServerRecvWindow(const wchar_t* server_guid);

private:
    // 委托处理器
    Delegate* delegate_;

    // server 的接收窗口
    HWND server_recv_window_;
};
