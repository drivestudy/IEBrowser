#pragma once

#include "ipc.h"

class IPCBuffer;

///
// 用于一对多的进程间通信，（一个 server 进程和多个 client 进程通信）
///
class IPCServer :
    public IPC::Delegate
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
        // 收到消息
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
    // 向指定 client 发送一条命令
    ///
    bool SendIPCMessage(
        unsigned int client_id,
        unsigned int message,
        std::shared_ptr<IPCBuffer> data,
        unsigned int time_out);

    ///
    // 向指定 client 投递一条命令
    ///
    bool PostIPCMessage(
        unsigned int client_id,
        unsigned int message,
        std::shared_ptr<IPCBuffer> data);

private:
    // 以下实现 IPC::Delegate 接口的方法

    ///
    // 收到了对端发来的 IPC 消息
    ///
    void OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data);

private:
    // 委托处理器
    Delegate* delegate_;

    // 维护 client_id 和 消息窗口 关系的表，发送消息时，先通过 client_id 找到对端消息窗口，才能发送
    std::map<unsigned int, HWND> client_window_map_;
};
