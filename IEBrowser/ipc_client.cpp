#include "stdafx.h"
#include "ipc_client.h"

#include "ipc.h"
#include "ipc_buffer.h"
#include "ipc_message_define.h"

// 接收窗口的窗口类名和窗口名
static const wchar_t* kRecvWindowClass = L"IPCClientRecvWindowClass";
static const wchar_t* kRecvWindow = L"IPCClientRecvWindow";

IPCClient::IPCClient() :
    delegate_(nullptr),
    server_recv_window_(nullptr)
{
}

IPCClient::~IPCClient()
{
}

IPCClient * IPCClient::GetInstance()
{
    static IPCClient instance;
    return &instance;
}

bool IPCClient::Connect(const wchar_t* server_guid)
{
    bool result = false;

    do
    {
        IPC* ipc = IPC::GetInstance();
        if (!ipc->Initialize(kRecvWindowClass, kRecvWindow))
        {
            break;
        }

        ipc->SetDelegate(this);

        HWND server_recv_window = FindServerRecvWindow(server_guid);
        if (server_recv_window == nullptr || !::IsWindow(server_recv_window))
        {
            break;
        }
        server_recv_window_ = server_recv_window;

        PostIPCMessage(WM_IPC_CLIENT_CONNECT, nullptr);

        result = true;

    } while (false);

    return result;
}

void IPCClient::DisConnect()
{
    PostIPCMessage(WM_IPC_CLIENT_DISCONNECT, nullptr);

    server_recv_window_ = nullptr;

    IPC* ipc = IPC::GetInstance();
    ipc->UnInitialize();
}

void IPCClient::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

bool IPCClient::PostIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data)
{
    bool result = false;

    do
    {
        if (!::IsWindow(server_recv_window_))
        {
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->PostIPCMessage(server_recv_window_, message, data);

    } while (false);

    return result;
}

bool IPCClient::SendIPCMessage(
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data,
    unsigned int time_out)
{
    bool result = false;

    do
    {
        if (!::IsWindow(server_recv_window_))
        {
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->SendIPCMessage(server_recv_window_, message, data, time_out);

    } while (false);

    return result;
}

HWND IPCClient::FindServerRecvWindow(const wchar_t * server_guid)
{
    return ::FindWindowEx(HWND_MESSAGE, nullptr, nullptr, server_guid);
}
