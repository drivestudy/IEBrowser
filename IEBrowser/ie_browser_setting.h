#pragma once

///
// ��ʼ�� IEBrowser ʱ��������
///
struct IEBrowserSetting
{
    IEBrowserSetting() :
        parent_window_handle_(nullptr)
    {
    }

    HWND parent_window_handle_;
};
