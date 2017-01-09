#pragma once

///
// 用于一对多的进程间通信，（一个 server 进程和多个 client 进程通信）
// 封装 Server 进程上的通信逻辑
///
class IPCServer
{
public:
    class Delegate
    {
    public:
        ///
        // 与某个 client 建立了连接
        ///
        void OnClientConnected(unsigned int client_id) {};

        ///
        // 某个 client 与 server 断开了连接
        ///
        void OnClientDisConnected(unsigned int client_id) {};

        ///
        // 收到来自 client 的命令
        ///
        void OnReceiveClientCommand(
            unsigned int client_id,
            unsigned int command_id,
            void* data,
            size_t data_size) {};
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
    // 启动 server
    ///
    bool Start();

    ///
    // 关闭 server
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
        void* data,
        size_t data_size,
        unsigned int time_out);

    ///
    // 向指定 client 投递一条命令
    ///
    bool PostCommand(
        unsigned int client_id,
        unsigned int command_id,
        void* data,
        size_t data_size);

private:
    // 委托处理器
    Delegate* delegate_;
};
