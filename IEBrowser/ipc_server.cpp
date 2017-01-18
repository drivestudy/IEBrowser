#include "stdafx.h"
#include "ipc_server.h"

#include "ipc_message_define.h"
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
        PostIPCMessage(0, WM_IPC_SEND_THREAD_QUIT, nullptr);

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

bool IPCServer::SendIPCMessage(
    unsigned int client_id, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data, 
    unsigned int time_out)
{
    bool result = false;

    do
    {
        if (!DoPostIPCMessage(client_id, message, data, WM_IPC_EX_PLEASE_REPLY))
        {
            break;
        }

        WaitForClientMessage(client_id, message, WM_IPC_EX_ROGER_THAT, time_out);

        result = true;

    } while (false);

    return result;
}

bool IPCServer::PostIPCMessage(
    unsigned int client_id, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data)
{
    return DoPostIPCMessage(client_id, message, data, 0);
}

bool IPCServer::DoPostIPCMessage(
    unsigned int client_id, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data, 
    unsigned int ex_message)
{
    bool result = false;

    do
    {
        auto iter = client_window_map_.find(client_id);
        if (iter == client_window_map_.end())
        {
            break;
        }

        HWND client_window = iter->second;
        if (!::IsWindow(client_window))
        {
            break;
        }

        // data 的前两个项用来存储具体的消息 id
        data->PushFront(IPCValue(message));
        data->PushFront(IPCValue(ex_message));

        std::shared_ptr<MessageInfo> message_info(new MessageInfo);
        message_info->client_id = client_id;
        message_info->message = message;
        message_info->client_window = client_window;
        message_info->data = data;
        message_info->ex_message = ex_message;

        send_queue_.Push(message_info);

        result = true;

    } while (false);

    return result;
}

bool IPCServer::WaitForClientMessage(
    unsigned int client_id, 
    unsigned int message, 
    unsigned int ex_message,
    unsigned int time_out)
{
    bool result = false;

    UINT_PTR timer_id = ::SetTimer(nullptr, 0, time_out, nullptr);

    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == message && 
            msg.wParam == (WPARAM)client_id && 
            msg.lParam == (LPARAM)ex_message)
        {
            break;
        }

        if (msg.message == WM_TIMER)
        {
            break;
        }
    }

    ::KillTimer(nullptr, timer_id);

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
        std::shared_ptr<MessageInfo> message_info = self->send_queue_.Take();
        if (message_info->message == WM_IPC_SEND_THREAD_QUIT)
        {
            break;
        }

        DoSendIPCMessage(message_info);
    }
}

bool IPCServer::DoSendIPCMessage(std::shared_ptr<MessageInfo> message_info)
{
    bool result = false;
    char* data = nullptr;

    do
    {
        HWND client_window = message_info->client_window;
        if (!::IsWindow(client_window))
        {
            break;
        }

        if (message_info->data == nullptr)
        {
            break;
        }

        size_t data_size = 0;
        if (!message_info->data->Encode(data, data_size))
        {
            break;
        }

        COPYDATASTRUCT copy_data;
        copy_data.lpData = data;
        copy_data.cbData = data_size;

        if (::SendMessageTimeout(
            client_window, 
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
