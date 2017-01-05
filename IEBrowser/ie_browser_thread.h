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
private:
    ///
    // 线程当前状态
    ///
    enum ThreadState
    {
        // 尚未初始化
        THREAD_STATE_UNINIT,

        // 初始化成功
        THREAD_STATE_INIT_SUCCEED,

        // 初始化失败
        THREAD_STATE_INIT_FAILED,
    };

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
    static void BrowserThreadProc(std::shared_ptr<IEBrowserThread> self, IEBrowserSetting setting);

    ///
    // 初始化工作线程，必须在工作线程里调用
    ///
    bool InitBrowserThread(IEBrowserSetting& setting);

private:
    // 线程对象
    std::unique_ptr<std::thread> thread_;

    // 线程当前状态
    ThreadState thread_state_;

    // 互斥量，用于保护 thread_state_ 成员变量
    std::mutex thread_state_mutex_;

    // 条件变量，用于等待 thread_state_ 的状态切换
    std::condition_variable thread_state_condition_;

    // 线程中运行的 IEBrowser 对象
    std::unique_ptr<IEBrowser> browser_;
};
