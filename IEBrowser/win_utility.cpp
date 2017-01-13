#include "stdafx.h"
#include "win_utility.h"

const wchar_t* kFrameWindowClassName = L"FrameWindowClass";

LRESULT CALLBACK DefaultWindowProc(
    HWND window_handle, 
    UINT message, 
    WPARAM w_param, 
    LPARAM l_param)
{
    return ::DefWindowProc(window_handle, message, w_param, l_param);
}

HWND WinUtility::CreateFrameWindow(const wchar_t * window_name)
{
    HWND window_handle = nullptr;

    do
    {
        // 注册窗口类
        WNDCLASSEX window_class = { 0 };
        HINSTANCE instance_handle = ::GetModuleHandle(NULL);
        if (::GetClassInfoEx(instance_handle, kFrameWindowClassName, &window_class) == FALSE)
        {
            ::ZeroMemory(&window_class, sizeof(window_class));
            window_class.cbSize = sizeof(window_class);
            window_class.lpfnWndProc = DefaultWindowProc;
            window_class.hInstance = instance_handle;
            window_class.lpszClassName = kFrameWindowClassName;

            if (::RegisterClassEx(&window_class) == 0)
            {
                break;
            }
        }

        // 创建窗口
        window_handle = ::CreateWindowEx(
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
            kFrameWindowClassName,
            window_name,
            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr,
            nullptr,
            instance_handle,
            NULL);

        if (window_handle == NULL)
        {
            break;
        }

        // 显示窗口
        ::ShowWindow(window_handle, SW_SHOW);
        ::UpdateWindow(window_handle);

    } while (false);

    return window_handle;
}

HWND WinUtility::CreateMessageWindow(
    WNDPROC window_proc, 
    const wchar_t * class_name, 
    const wchar_t * window_name)
{
    HWND window_handle = nullptr;

    do
    {
        // 注册窗口类
        WNDCLASSEXW window_class;
        HINSTANCE instance_handle = ::GetModuleHandle(NULL);
        if (::GetClassInfoExW(instance_handle, class_name, &window_class) == FALSE)
        {
            ::ZeroMemory(&window_class, sizeof(window_class));
            window_class.cbSize = sizeof(window_class);
            window_class.lpfnWndProc = window_proc;
            window_class.hInstance = instance_handle;
            window_class.lpszClassName = class_name;

            if (::RegisterClassExW(&window_class) == 0)
            {
                break;
            }
        }

        // 创建窗口
        window_handle = ::CreateWindowW(
            class_name,
            window_name,
            0,
            0, 0, 0, 0,
            HWND_MESSAGE,
            0,
            instance_handle,
            0);

        if (window_handle == 0)
        {
            break;
        }

    } while (false);

    return window_handle;
}
