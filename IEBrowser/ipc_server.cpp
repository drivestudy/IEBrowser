#include "stdafx.h"
#include "ipc_server.h"

#include "ipc.h"
#include "ipc_buffer.h"
#include "ipc_message_define.h"

// 接收窗口的窗口类名
const wchar_t* kRecvWindowClass = L"IPCServerRecvWindowClass";

// 调用 SendMessageTimeout 时的超时
const int kSendMessageTimeout = 5000;

IPCServer::IPCServer() :
    delegate_(nullptr)
{
}

IPCServer::~IPCServer()
{
}

IPCServer * IPCServer::GetInstance()
{
    static IPCServer instance;
    return &instance;
}

bool IPCServer::Start(const wchar_t* server_guid)
{
    bool result = false;

    do
    {
        IPC* ipc = IPC::GetInstance();
        if (!ipc->Initialize(kRecvWindowClass, server_guid))
        {
            break;
        }

        ipc->SetDelegate(this);

        result = true;

    } while (false);

    return result;
}

void IPCServer::Stop()
{
    IPC* ipc = IPC::GetInstance();
    ipc->UnInitialize();
}

void IPCServer::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

bool IPCServer::SendIPCMessage(
    unsigned int client_id, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data, 
    unsigned int time_out)
{
    bool result = false;

    do
    {
        auto iter = client_window_map_.find(client_id);
        if (iter == client_window_map_.end())
        {
            break;
        }

        HWND recv_window = iter->second;
        if (!::IsWindow(recv_window))
        {
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->SendIPCMessage(recv_window, message, data, time_out);

    } while (false);

    return result;
}

bool IPCServer::PostIPCMessage(
    unsigned int client_id, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data)
{
    bool result = false;

    do
    {
        auto iter = client_window_map_.find(client_id);
        if (iter == client_window_map_.end())
        {
            break;
        }

        HWND recv_window = iter->second;
        if (!::IsWindow(recv_window))
        {
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->PostIPCMessage(recv_window, message, data);

    } while (false);

    return result;
}

void IPCServer::OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data)
{
    do
    {
        if (delegate_ == nullptr)
        {
            break;
        }

        IPCValue client_id_value = data->TakeFront();
        if (client_id_value.GetType() != IPCValue::VT_UINT)
        {
            break;
        }
        unsigned int client_id = client_id_value.GetUInt();

        delegate_->OnRecvIPCMessage(client_id, message, data);

    } while (false);
}
