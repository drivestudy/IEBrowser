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
    LOG_ENTER_EX(L"recv_window_class_name = %s, recv_window_name = %s", recv_window_class_name, recv_window_name);

    bool result = false;

    do
    {
        // 创建接收窗口
        if (!CreateRecvWindow(recv_window_class_name, recv_window_name))
        {
            LOG_ERROR(L"CreateRecvWindow failed!");
            break;
        }

        // 创建发送线程
        if (!CreateSendThread())
        {
            LOG_ERROR(L"CreateSendThread failed!");
            break;
        }

        result = true;

    } while (false);

    if (!result)
    {
        UnInitialize();
    }

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

void IPC::UnInitialize()
{
    LOG_ENTER();

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

    LOG_EXIT();
}

void IPC::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

HWND IPC::GetRecvWindow()
{
    return recv_window_;
}

bool IPC::PostIPCMessage(HWND recv_window, unsigned int message, std::shared_ptr<IPCBuffer> data)
{
    LOG_ENTER_EX(L"recv_window = 0x%08x, message = %d", recv_window, message);

    bool result = DoPostIPCMessage(recv_window, message, 0, data);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

bool IPC::SendIPCMessage(
    HWND recv_window, 
    unsigned int message, 
    std::shared_ptr<IPCBuffer> data,
    unsigned int time_out)
{
    LOG_ENTER_EX(L"recv_window = 0x%08x, message = %d, time_out = %d", recv_window, message, time_out);

    bool result = false;

    do
    {
        if (!DoPostIPCMessage(recv_window, message, WM_IPC_EX_PLEASE_REPLY, data))
        {
            LOG_ERROR(L"DoPostIPCMessage failed!");
            break;
        }

        WaitForNotifyMessage(recv_window, message, WM_IPC_EX_ROGER_THAT, time_out);

        result = true;

    } while (false);

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

bool IPC::CreateSendThread()
{
    LOG_ENTER();

    send_thread_.reset(new std::thread(IPC::SendThreadProc));

    LOG_EXIT();

    return true;
}

void IPC::SendThreadProc()
{
    LOG_ENTER();

    IPC* self = IPC::GetInstance();

    while (true)
    {
        std::shared_ptr<MessageInfo> message_info = self->send_queue_.Take();
        if (message_info->message == WM_IPC_SEND_THREAD_QUIT)
        {
            LOG_CRITICAL(L"recv quit message, break");
            break;
        }

        DoSendIPCMessage(message_info->recv_window, message_info->data);
    }

    LOG_EXIT();
}

bool IPC::CreateRecvWindow(const wchar_t * recv_window_class_name, const wchar_t * recv_window_name)
{
    LOG_ENTER_EX(L"recv_window_class_name = %s, recv_window_name = %s", recv_window_class_name, recv_window_name);

    bool result = false;

    do
    {
        recv_window_ = WinUtility::CreateMessageWindow(
            IPC::RecvWindowProc,
            recv_window_class_name,
            recv_window_name);

        if (recv_window_ == nullptr)
        {
            LOG_ERROR(L"CreateMessageWindow failed!");
            break;
        }

        result = true;

    } while (false);

    LOG_EXIT_EX(L"result = %d, recv_window_ = 0x%08x", result, recv_window_);

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

        LOG_INFO(L"receive WM_COPYDATA");

        // 没有定义 delegate 的话，不需要处理这条消息了
        IPC* self = IPC::GetInstance();
        if (self->delegate_ == nullptr)
        {
            LOG_ERROR(L"no delegate");
            break;
        }

        // 从 copy data 中解码出发来的数据
        PCOPYDATASTRUCT copy_data = (PCOPYDATASTRUCT)l_param;
        if (copy_data == nullptr)
        {
            LOG_ERROR(L"copy_data == nullptr");
            break;
        }

        char* buffer = (char*)copy_data->lpData;
        size_t buffer_size = copy_data->cbData;
        if (buffer == nullptr || buffer_size == 0)
        {
            LOG_ERROR(L"buffer not valid, buffer = 0x%08x, buffer_size = %d", buffer, buffer_size);
            break;
        }

        std::shared_ptr<IPCBuffer> data(new IPCBuffer);
        if (!data->Decode(buffer, buffer_size))
        {
            LOG_ERROR(L"Decode failed!");
            break;
        }

        // data 的前两项是 message 和 internal_message
        IPCValue message_value = data->TakeFront();
        if (message_value.GetType() != IPCValue::VT_UINT)
        {
            LOG_ERROR(L"take message failed!");
            break;
        }
        unsigned int recv_message = message_value.GetUInt();

        IPCValue internal_message_value = data->TakeFront();
        if (internal_message_value.GetType() != IPCValue::VT_UINT)
        {
            LOG_ERROR(L"take internal_message failed!");
            break;
        }
        unsigned int internal_message = internal_message_value.GetUInt();

        LOG_INFO(L"recv message, message = %d, internal_message = %d", recv_message, internal_message);

        // 触发给使用者
        self->delegate_->OnRecvIPCMessage(recv_message, data);

        // 如果发送方需要回复，则回复一条 ROGER_THAT 消息，表示已经处理了对端的消息
        if (internal_message == WM_IPC_EX_PLEASE_REPLY)
        {
            HWND send_window = (HWND)w_param;
            if (send_window == nullptr || !::IsWindow(send_window))
            {
                LOG_ERROR(L"please reply but send_window is not valid");
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
    LOG_ENTER_EX(L"recv_window = 0x%08x, message = %d, internal_message = %d", recv_window, message, internal_message);

    bool result = false;

    do
    {
        if (!::IsWindow(recv_window))
        {
            LOG_ERROR(L"recv_window is not window");
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

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

bool IPC::DoSendIPCMessage(HWND recv_window, std::shared_ptr<IPCBuffer> message_data)
{
    LOG_ENTER_EX(L"recv_window = 0x%08x", recv_window);

    bool result = false;
    char* buffer = nullptr;

    do
    {
        IPC* self = IPC::GetInstance();

        if (!::IsWindow(recv_window))
        {
            LOG_ERROR(L"recv_window is not window");
            break;
        }

        if (message_data == nullptr)
        {
            LOG_ERROR(L"message_data == nullptr");
            break;
        }

        size_t buffer_size = 0;
        if (!message_data->Encode(buffer, buffer_size))
        {
            LOG_ERROR(L"Encode failed");
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
            LOG_ERROR(L"SendMessageTimeout failed");
            break;
        }

        result = true;

    } while (false);

    if (buffer)
    {
        delete[] buffer;
        buffer = nullptr;
    }

    LOG_EXIT_EX(L"result = %d", result);

    return result;
}

void IPC::WaitForNotifyMessage(
    HWND send_window, 
    unsigned int message, 
    unsigned int internal_message, 
    unsigned int time_out)
{
    LOG_ENTER_EX(L"send_window = 0x%08x", send_window);

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
            LOG_INFO(L"recv message, break");
            break;
        }

        if (msg.message == WM_TIMER)
        {
            LOG_ERROR(L"timeout, break");
            break;
        }
    }

    ::KillTimer(nullptr, timer_id);

    LOG_EXIT();
}
