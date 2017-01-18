#pragma once

// IPC 模块的窗口消息
enum IPCMessage
{
    // 停止发送线程
    WM_IPC_SEND_THREAD_QUIT = WM_USER + 1,

    // 指明此命令需要回复
    WM_IPC_EX_PLEASE_REPLY,

    // 指明此条消息已经收到
    WM_IPC_EX_ROGER_THAT,

    // client 发送的连接请求
    WM_IPC_CLIENT_CONNECT,

    // client 发送的断开连接请求
    WM_IPC_CLIENT_DISCONNECT,
};