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
    do
    {
        if (message != WM_COPYDATA)
        {
            break;
        }

        // 没有定义 delegate 的话，不需要处理这条消息了
        IPC* self = IPC::GetInstance();
        if (self->delegate_ == nullptr)
        {
            break;
        }

        // 从 copy data 中解码出发来的数据
        PCOPYDATASTRUCT copy_data = (PCOPYDATASTRUCT)l_param;
        if (copy_data == nullptr)
        {
            break;
        }

        char* buffer = (char*)copy_data->lpData;
        size_t buffer_size = copy_data->cbData;
        if (buffer == nullptr || buffer_size == 0)
        {
            break;
        }

        std::shared_ptr<IPCBuffer> data(new IPCBuffer);
        if (!data->Decode(buffer, buffer_size))
        {
            break;
        }

        // data 的前两项是 message 和 internal_message
        IPCValue message_value = data->TakeFront();
        if (message_value.GetType() != IPCValue::VT_UINT)
        {
            break;
        }
        unsigned int recv_message = message_value.GetUInt();

        IPCValue ex_message_value = data->TakeFront();
        if (ex_message_value.GetType() != IPCValue::VT_UINT)
        {
            break;
        }
        unsigned int internal_message = ex_message_value.GetUInt();

        // 触发给使用者
        self->delegate_->OnRecvIPCMessage(recv_message, data);

        // 如果发送方需要回复，则回复一条 ROGER_THAT 消息，表示已经处理了对端的消息
        if (internal_message == WM_IPC_EX_PLEASE_REPLY)
        {
            HWND send_window = (HWND)w_param;
            if (send_window == nullptr || !::IsWindow(send_window))
            {
                break;
            }

            ::PostMessage(send_window, recv_message, (WPARAM)self->recv_window_, WM_IPC_EX_ROGER_THAT);
        }

    } while (false);

    return ::DefWindowProc(window_handle, message, w_param, l_param);
}

bool IPC::DoPostIPCMessage(
    HWND recv_window, 
    unsigned int message, 
    unsigned int internal_message, 
    std::shared_ptr<IPCBuffer> data)
{
    bool result = false;

    do
    {
        if (!::IsWindow(recv_window))
        {
            break;
        }

        if (data == nullptr)
        {
            data.reset(new IPCBuffer());
        }

        // data 的前两个项用来存储具体的消息 id
        data->PushFront(IPCValue(internal_message));
        data->PushFront(IPCValue(message));

        std::shared_ptr<MessageInfo> message_info(new MessageInfo);
        message_info->recv_window = recv_window;
        message_info->message = message;
        message_info->internal_message = internal_message;
        message_info->data = data;

        send_queue_.Push(message_info);

        result = true;

    } while (false);

    return result;
}

bool IPC::DoSendIPCMessage(HWND recv_window, std::shared_ptr<IPCBuffer> message_data)
{
    bool result = false;
    char* buffer = nullptr;

    do
    {
        IPC* self = IPC::GetInstance();

        if (!::IsWindow(recv_window))
        {
            break;
        }

        if (message_data == nullptr)
        {
            break;
        }

        size_t buffer_size = 0;
        if (!message_data->Encode(buffer, buffer_size))
        {
            break;
        }

        COPYDATASTRUCT copy_data;
        copy_data.lpData = buffer;
        copy_data.cbData = buffer_size;

        if (::SendMessageTimeout(
            recv_window,
            WM_COPYDATA,
            (WPARAM)self->recv_window_,
            (LPARAM)&copy_data,
            SMTO_ABORTIFHUNG,
            kSendMessageTimeout,
            nullptr) == 0)
        {
            break;
        }

        result = true;

    } while (false);

    if (buffer)
    {
        delete[] buffer;
        buffer = nullptr;
    }

    return result;
}

void IPC::WaitForNotifyMessage(
    HWND send_window, 
    unsigned int message, 
    unsigned int internal_message, 
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
            msg.lParam == (LPARAM)internal_message)
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
