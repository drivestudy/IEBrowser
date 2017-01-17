#pragma once

enum IPCCommand
{
    // 停止发送线程
    CM_SEND_THREAD_QUIT,

    // 指明此命令需要回复
    CM_EX_PLEASE_REPLY,
};

// IPC 模块的窗口消息
enum IPCMessage
{
    // client 发来的消息，表示某条命令已经收到
    WM_IPC_CLIENT_ROGER_THAT = WM_USER + 1,
};