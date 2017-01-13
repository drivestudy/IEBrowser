/******************************************************************************
*
* @description: 封装一些 Windows 平台相关的常用操作
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

namespace WinUtility
{
    ///
    // 创建一个普通的 frame 窗口
    ///
    HWND CreateFrameWindow(const wchar_t* window_name = L"FrameWindow");

    ///
    // 创建一个消息窗口
    ///
    HWND CreateMessageWindow(
        WNDPROC window_proc,
        const wchar_t* class_name = L"MessageWindowClass",
        const wchar_t* window_name = L"MessageWindow");
}
