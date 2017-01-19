#include "stdafx.h"
#include "ipc_server.h"

#include "ipc.h"
#include "ipc_buffer.h"
#include "ipc_message_define.h"

// 接收窗口的窗口类名
static const wchar_t* kRecvWindowClass = L"IPCServerRecvWindowClass";

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
    LOG_ENTER_EX(L" server_guid = %s", server_guid);

    bool result = false;

    do
    {
        // 初始化 ipc
        IPC* ipc = IPC::GetInstance();
        if (!ipc->Initialize(kRecvWindowClass, server_guid))
        {
            LOG_ERROR(L"IPC::Initialize failed");
            break;
        }

        ipc->SetDelegate(this);

        result = true;

    } while (false);

    LOG_INFO(L"Exit result = %d", result);

    return result;
}

void IPCServer::Stop()
{
    LOG_ENTER();

    IPC* ipc = IPC::GetInstance();
    ipc->UnInitialize();

    LOG_EXIT();
}

void IPCServer::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

bool IPCServer::PostIPCMessage(
    unsigned int client_id,
    unsigned int message,
    std::shared_ptr<IPCBuffer> data)
{
    LOG_ENTER_EX(L"client_id = %d, message = %d", client_id, message);

    bool result = false;

    do
    {
        auto iter = client_window_map_.find(client_id);
        if (iter == client_window_map_.end())
        {
            LOG_ERROR(L"client_id not found");
            break;
        }

        HWND recv_window = iter->second;
        if (!::IsWindow(recv_window))
        {
            LOG_ERROR(L"recv_window is not window");
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->PostIPCMessage(recv_window, message, data);

    } while (false);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

bool IPCServer::SendIPCMessage(
    unsigned int client_id, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data, 
    unsigned int time_out)
{
    LOG_ENTER_EX(L"client_id = %d, message = %d, time_out = %d", client_id, message, time_out);

    bool result = false;

    do
    {
        auto iter = client_window_map_.find(client_id);
        if (iter == client_window_map_.end())
        {
            LOG_ERROR(L"client_id not found");
            break;
        }

        HWND recv_window = iter->second;
        if (!::IsWindow(recv_window))
        {
            LOG_ERROR(L"recv_window is not window");
            break;
        }

        IPC* ipc = IPC::GetInstance();
        result = ipc->SendIPCMessage(recv_window, message, data, time_out);

    } while (false);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

void IPCServer::OnRecvIPCMessage(unsigned int message, std::shared_ptr<IPCBuffer> data)
{
    LOG_ENTER_EX(L"message = %d", message);

    do
    {
        if (delegate_ == nullptr)
        {
            LOG_ERROR(L"delegate == nullptr");
            break;
        }

        // 获取 client_id
        IPCValue client_id_value = data->TakeFront();
        if (client_id_value.GetType() != IPCValue::VT_UINT)
        {
            LOG_ERROR(L"take client id failed!");
            break;
        }
        unsigned int client_id = client_id_value.GetUInt();

        switch (message)
        {
        case WM_IPC_CLIENT_CONNECT:
        {
            // 获取 client_window
            IPCValue client_window_value = data->TakeFront();
            if (client_window_value.GetType() != IPCValue::VT_INT)
            {
                LOG_ERROR(L"take client_recv_window failed!");
                break;
            }

            HWND client_window = (HWND)client_window_value.GetInt();
            if (!::IsWindow(client_window))
            {
                LOG_ERROR(L"client_window is not window, client_window = 0x%08x", client_window);
                break;
            }

            // 保存 client_window
            client_window_map_[client_id] = client_window;

            // 触发委托
            delegate_->OnClientConnected(client_id);
            break;
        }
        case WM_IPC_CLIENT_DISCONNECT:
        {
            delegate_->OnClientDisConnected(client_id);
            break;
        }
        default:
        {
            delegate_->OnRecvIPCMessage(client_id, message, data);
            break;
        }
        }

    } while (false);

    LOG_EXIT();
}
