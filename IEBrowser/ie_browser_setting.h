/******************************************************************************
*
* @description: IEBrowser 对象的设置项，封装了传递给 IEBrowser::Initialize() 的参数
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

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
