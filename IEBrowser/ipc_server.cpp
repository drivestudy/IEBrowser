#include "stdafx.h"
#include "ipc_server.h"

#include "ipc_command_define.h"
#include "win_utility.h"

// 接收窗口的窗口类名
const wchar_t* kRecvWindowClass = L"IPCServerRecvWindowClass";

// 调用 SendMessageTimeout 时的超时
const int kSendMsgTimeout = 5000;

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
    bool result = false;

    do
    {
        auto iter = client_message_window_map_.find(client_id);
        if (iter == client_message_window_map_.end())
        {
            break;
        }

        HWND client_message_window = iter->second;
        if (!::IsWindow(client_message_window))
        {
            break;
        }

        std::shared_ptr<CommandInfo> command_info(new CommandInfo);
        command_info->client_id = client_id;
        command_info->command_id = command_id;
        command_info->client_message_window = client_message_window;
        command_info->data = buffer;

        send_queue_.Push(command_info);

        result = true;

    } while (false);

    return result;
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
        std::shared_ptr<CommandInfo> command_info = self->send_queue_.Take();
        if (command_info->command_id == CM_SEND_THREAD_QUIT)
        {
            break;
        }

        DoSendCommand(command_info);
    }
}

bool IPCServer::DoSendCommand(std::shared_ptr<CommandInfo> command_info)
{
    bool result = false;
    char* data = nullptr;

    do
    {
        HWND client_message_window = command_info->client_message_window;
        if (!::IsWindow(client_message_window))
        {
            break;
        }

        if (command_info->data == nullptr)
        {
            break;
        }

        size_t data_size = 0;
        if (!command_info->data->Encode(data, data_size))
        {
            break;
        }

        COPYDATASTRUCT copy_data;
        copy_data.lpData = data;
        copy_data.cbData = data_size;

        if (::SendMessageTimeout(
            client_message_window, 
            WM_COPYDATA,
            (WPARAM)nullptr,
            (LPARAM)&copy_data,
            SMTO_ABORTIFHUNG,
            kSendMsgTimeout,
            nullptr) == 0)
        {
            break;
        }

        result = true;

    } while (false);

    if (data)
    {
        delete[] data;
        data = nullptr;
    }

    return result;
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
