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
};