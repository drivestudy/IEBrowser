#include "stdafx.h"
#include "ipc.h"

#include "ipc_buffer.h"
#include "ipc_message_define.h"
#include "win_utility.h"

// 调用 SendMessageTimeout 时的超时
const int kSendMessageTimeout = 5000;

IPC::IPC() :
    delegate_(nullptr)
{
}

IPC::~IPC()
{
}

IPC * IPC::GetInstance()
{
    static IPC instance;
    return &instance;
}

bool IPC::Initialize(const wchar_t * recv_window_class_name, const wchar_t * recv_window_name)
{
    bool result = false;

    do
    {
        // 创建接收窗口
        if (!CreateRecvWindow(recv_window_class_name, recv_window_name))
        {
            break;
        }

        // 创建发送线程
        if (!CreateSendThread())
        {
            break;
        }

        result = true;

    } while (false);

    if (!result)
    {
        UnInitialize();
    }

    return result;
}

void IPC::UnInitialize()
{
    if (recv_window_)
    {
        ::DestroyWindow(recv_window_);
        recv_window_ = nullptr;
    }

    if (send_thread_)
    {
        // 向线程发送退出消息
        std::shared_ptr<MessageInfo> message_info;
        message_info->message = WM_IPC_SEND_THREAD_QUIT;
        send_queue_.Push(message_info);

        // 等待线程终止
        send_thread_->join();
        send_thread_.reset(nullptr);
    }
}

void IPC::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

bool IPC::PostIPCMessage(HWND recv_window, unsigned int message, std::shared_ptr<IPCBuffer> data)
{
    return DoPostIPCMessage(recv_window, message, 0, data);
}

bool IPC::SendIPCMessage(
    HWND recv_window, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data,
    unsigned int time_out)
{
    bool result = false;

    do
    {
        if (!DoPostIPCMessage(recv_window, message, WM_IPC_EX_PLEASE_REPLY, data))
        {
            break;
        }

        WaitForNotifyMessage(recv_window, message, WM_IPC_EX_ROGER_THAT, time_out);

        result = true;

    } while (false);

    return result;
}

bool IPC::CreateSendThread()
{
    send_thread_.reset(new std::thread(IPC::SendThreadProc));
    return true;
}

void IPC::SendThreadProc()
{
    IPC* self = IPC::GetInstance();

    while (true)
    {
        std::shared_ptr<MessageInfo> message_info = self->send_queue_.Take();
        if (message_info->message == WM_IPC_SEND_THREAD_QUIT)
        {
            break;
        }

        DoSendIPCMessage(message_info->recv_window, message_info->data);
    }
}

bool IPC::CreateRecvWindow(const wchar_t * recv_window_class_name, const wchar_t * recv_window_name)
{
    bool result = false;

    do
    {
        recv_window_ = WinUtility::CreateMessageWindow(
            IPC::RecvWindowProc,
            recv_window_class_name,
            recv_window_name);

        if (recv_window_ == nullptr)
        {
            break;
        }

        result = true;

    } while (false);

    return result;
}

LRESULT IPC::RecvWindowProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
    return ::DefWindowProc(window_handle, message, w_param, l_param);
}

bool IPC::DoPostIPCMessage(
    HWND recv_window, 
    unsigned int message, 
    unsigned int ex_message, 
    std::shared_ptr<IPCBuffer> data)
{
    bool result = false;

    do
    {
        if (!::IsWindow(recv_window))
        {
            break;
        }

        // data 的前两个项用来存储具体的消息 id
        data->PushFront(IPCValue(message));
        data->PushFront(IPCValue(ex_message));

        std::shared_ptr<MessageInfo> message_info(new MessageInfo);
        message_info->recv_window = recv_window;
        message_info->message = message;
        message_info->ex_message = ex_message;
        message_info->data = data;

        send_queue_.Push(message_info);

        result = true;

    } while (false);

    return result;
}

bool IPC::DoSendIPCMessage(HWND recv_window, std::shared_ptr<IPCBuffer> message_data)
{
    bool result = false;
    char* data = nullptr;

    do
    {
        if (!::IsWindow(recv_window))
        {
            break;
        }

        if (message_data == nullptr)
        {
            break;
        }

        size_t data_size = 0;
        if (!message_data->Encode(data, data_size))
        {
            break;
        }

        COPYDATASTRUCT copy_data;
        copy_data.lpData = data;
        copy_data.cbData = data_size;

        if (::SendMessageTimeout(
            recv_window,
            WM_COPYDATA,
            (WPARAM)nullptr,
            (LPARAM)&copy_data,
            SMTO_ABORTIFHUNG,
            kSendMessageTimeout,
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

void IPC::WaitForNotifyMessage(
    HWND send_window, 
    unsigned int message, 
    unsigned int ex_message, 
    unsigned int time_out)
{
    UINT_PTR timer_id = ::SetTimer(nullptr, 0, time_out, nullptr);

    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (msg.message == message &&
            msg.wParam == (WPARAM)send_window &&
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
}
