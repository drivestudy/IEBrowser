/******************************************************************************
*
* @description: ��װһЩ Windows ƽ̨��صĳ��ò���
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

namespace WinUtility
{
    ///
    // ����һ����ͨ�� frame ����
    ///
    HWND CreateFrameWindow(const wchar_t* window_name = L"FrameWindow");

    ///
    // ����һ����Ϣ����
    ///
    HWND CreateMessageWindow(
        WNDPROC window_proc,
        const wchar_t* class_name = L"MessageWindowClass",
        const wchar_t* window_name = L"MessageWindow");
}
