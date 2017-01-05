#pragma once

///
// 初始化 IEBrowser 时的设置项
///
struct IEBrowserSetting
{
    IEBrowserSetting() :
        parent_window_handle_(nullptr)
    {
    }

    HWND parent_window_handle_;
};
