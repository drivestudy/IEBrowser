#include "stdafx.h"
#include "ipc_server.h"

#include "ipc_command_define.h"
#include "win_utility.h"

const wchar_t* kRecvWindowClass = L"IPCServerRecvWindowClass";

IPCServer::IPCServer() :
    recv_window_(nullptr),
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
        if (server_guid == nullptr)
        {
            break;
        }

        if (!CreateRecvWindow(server_guid))
        {
            break;
        }

        if (!CreateSendThread())
        {
            break;
        }

        result = true;

    } while (false);

    if (!result)
    {
        Stop();
    }

    return result;
}

void IPCServer::Stop()
{
    if (recv_window_)
    {
        ::DestroyWindow(recv_window_);
        recv_window_ = nullptr;
    }

    if (send_thread_)
    {
        // 向线程发送退出消息
        PostCommand(0, CM_SEND_THREAD_QUIT, nullptr);

        send_thread_->join();
        send_thread_.reset(nullptr);
    }
}

void IPCServer::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

void IPCServer::RemoveDelegate()
{
    delegate_ = nullptr;
}

bool IPCServer::SendCommand(
    unsigned int client_id, 
    unsigned int command_id, 
    std::shared_ptr<IPCBuffer> buffer, 
    unsigned int time_out)
{
    return false;
}

bool IPCServer::PostCommand(
    unsigned int client_id, 
    unsigned int command_id, 
    std::shared_ptr<IPCBuffer> buffer)
{
    CommandInfo command_info;
    command_info.client_id = client_id;
    command_info.command_id = command_id;
    command_info.data = buffer;

    send_queue_.Push(command_info);

    return true;
}

bool IPCServer::CreateSendThread()
{
    send_thread_.reset(new std::thread(IPCServer::SendThreadProc));
    return true;
}

void IPCServer::SendThreadProc()
{
    IPCServer* self = IPCServer::GetInstance();

    while (true)
    {
        CommandInfo command_info = self->send_queue_.Take();
        if (command_info.command_id == CM_SEND_THREAD_QUIT)
        {
            break;
        }

        // TODO : 进行发送操作
    }
}

bool IPCServer::CreateRecvWindow(const wchar_t* server_guid)
{
    bool result = false;

    do
    {
        recv_window_ = WinUtility::CreateMessageWindow(
            IPCServer::RecvWindowProc, 
            kRecvWindowClass, 
            server_guid);

        if (recv_window_ == nullptr)
        {
            break;
        }

        result = true;

    } while (false);

    return result;
}

LRESULT IPCServer::RecvWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
    return ::DefWindowProc(window_handle, message, w_param, l_param);
}
