/******************************************************************************
*
* @description: IE 浏览器线程类，为 IEBrowser 提供运行环境
*               每个 IEBrowser 运行在单独的线程中
*
* @author:      Dongyu
* @date:        2017.1.6
*
******************************************************************************/

#pragma once

#include "ie_browser_setting.h"

class IEBrowser;

///
// IE 浏览器线程类，为 IEBrowser 提供运行环境
// IEBrowserThread 对象代表一个单独的线程，其中运行了一个 IEBrowser
// IEBrowserThread 中包含了一个通信器，这个通信器会接收使用者发来的命令，并根据命令操纵 IEBrowser
///
class IEBrowserThread :
    public std::enable_shared_from_this<IEBrowserThread>
{
public:
    IEBrowserThread();
    ~IEBrowserThread();

    ///
    // 初始化
    ///
    bool Initialize(IEBrowserSetting& setting);

    ///
    // 反初始化
    ///
    void UnInitialize();

private:
    // 以下是私有函数

    ///
    // 线程函数，运行在单独线程中
    ///
    static void BrowserThreadProc(
        std::shared_ptr<IEBrowserThread> self, 
        IEBrowserSetting setting, 
        unsigned int init_thread_id);

    ///
    // 等待指定 Window 消息
    // 通过 while(::GetMessage()) 的方式来等待，不会阻塞调用方的消息循环
    ///
    void WaitForWindowMessage(UINT message, WPARAM* w_param = nullptr, LPARAM* l_param = nullptr);

private:

    // 线程对象
    std::unique_ptr<std::thread> thread_;

    // 线程中运行的 IEBrowser 对象
    std::unique_ptr<IEBrowser> browser_;
};
