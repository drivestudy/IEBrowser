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
    LOG_ENTER_EX(L"server_guid = %s", server_guid);

    bool result = false;

    do
    {
        // 初始化 ipc
        IPC* ipc = IPC::GetInstance();
        if (!ipc->Initialize(kRecvWindowClass, kRecvWindow))
        {
            LOG_ERROR(L"IPC::Initialize failed!");
            break;
        }

        ipc->SetDelegate(this);

        // 获取 server 的接收窗口句柄
        HWND server_recv_window = FindServerRecvWindow(server_guid);
        if (server_recv_window == nullptr || !::IsWindow(server_recv_window))
        {
            LOG_ERROR(L"FindServerRecvWindow failed");
            break;
        }
        server_recv_window_ = server_recv_window;

        // 连接到 server
        std::shared_ptr<IPCBuffer> data(new IPCBuffer);
        data->PushBack(IPCValue(GetClientID()));
        PostIPCMessage(WM_IPC_CLIENT_CONNECT, data);

        result = true;

    } while (false);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

void IPCClient::DisConnect()
{
    LOG_ENTER();

    PostIPCMessage(WM_IPC_CLIENT_DISCONNECT, nullptr);

    server_recv_window_ = nullptr;

    IPC* ipc = IPC::GetInstance();
    ipc->UnInitialize();

    LOG_EXIT();
}

void IPCClient::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

unsigned int IPCClient::GetClientID()
{
    return ::GetCurrentProcessId();
}

bool IPCClient::PostIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data)
{
    LOG_ENTER_EX(L"message = %d", message);

    bool result = false;

    do
    {
        if (!::IsWindow(server_recv_window_))
        {
            LOG_ERROR(L"server_recv_window_ is not valid, server_recv_window = 0x%08x", server_recv_window_);
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->PostIPCMessage(server_recv_window_, message, data);

    } while (false);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

bool IPCClient::SendIPCMessage(
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data,
    unsigned int time_out)
{
    LOG_ENTER_EX(L"message = %d, time_out = %d", message, time_out);

    bool result = false;

    do
    {
        if (!::IsWindow(server_recv_window_))
        {
            LOG_ERROR(L"server_recv_window_ is not window");
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->SendIPCMessage(server_recv_window_, message, data, time_out);

    } while (false);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

HWND IPCClient::FindServerRecvWindow(const wchar_t * server_guid)
{
    HWND server_window = ::FindWindowEx(HWND_MESSAGE, nullptr, nullptr, server_guid);

    LOG_INFO(L"server_window = 0x%08x", server_window);

    return server_window;
}
