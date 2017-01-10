#pragma once

///
// 用于一对多的进程间通信, (一个 server 进程和多个 client 进程)
// 封装 client 进程上的通信接口
///
class IPCClient
{
public:
    class Delegate
    {
    public:
        ///
        // 收到来自 server 的命令
        ///
        virtual void OnReceiveServerCommand(
            unsigned int command_id,
            void* data,
            size_t data_size) {};
    };

private:
    IPCClient();
    ~IPCClient();

    ///
    // 获取单例
    ///
    static IPCClient* GetInstance();

    ///
    // 连接到 server
    ///
    bool Connect();

    ///
    // 与 server 断开连接
    ///
    void DisConnect();

    ///
    // 设置委托
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // 移除委托
    ///
    void RemoveDelegate();

    ///
    // 向 server 发送一条命令
    ///
    bool SendCommand(unsigned int command_id, void* data, size_t data_size, unsigned int time_out);

    ///
    // 向 server 投递一条命令
    ///
    bool PostCommand(unsigned int command_id, void* data, size_t data_size);

private:
    // 委托处理器
    Delegate* delegate_;
};
