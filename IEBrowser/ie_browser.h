/******************************************************************************
*
* @description: IE 浏览器类，对 WebBrowser 控件进行封装
*
* @author:      Dongyu
* @date:        2017.1.4
*
******************************************************************************/

#pragma once

#include <ExDispid.h>

#define IEBROWSER_WINDOW_CLASS_NAME L"IEBrowserWindowClass"
#define IEBROWSER_WINDOW_NAME L"IEBrowserWindow"
#define IEBROWSER_DISP_ID 1

///
// IE 浏览器类，封装 WebBrowser 控件
///
class IEBrowser :
    public CWindowImpl<IEBrowser, CAxWindow>,
    public IDispEventImpl<IEBROWSER_DISP_ID, IEBrowser, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 0>
{
public:
    ///
    // 委托类，使用者继承这个类可以监听浏览器事件
    ///
    class Delegate
    {
    public:
        ///
        // 导航前触发的事件
        ///
        virtual void OnBeforeNavigate(
            const wchar_t* url, 
            const wchar_t* target_frame_name, 
            const wchar_t* post_data, 
            const wchar_t* headers) {};
    };

public:
    IEBrowser();
    ~IEBrowser();

    ///
    // 初始化
    ///
    bool Initialize(HWND parent_window_handle);

    ///
    // 反初始化
    ///
    void UnInitialze();

    ///
    // 添加委托处理者
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // 移除委托处理者
    ///
    void RemoveDelegate();

    ///
    // 导航一个 url
    ///
    bool Navigate(const wchar_t* url);

    ///
    // 停止正在进行的导航操作
    ///
    bool Stop();

    ///
    // 刷新当前页面
    ///
    bool Refresh();

    ///
    // 前进操作
    ///
    bool GoForward();

    ///
    // 后退操作
    ///
    bool GoBack();

public:
    // 超类化 CAxWindow, CAxWindow 可以作为 ActiveX 控件的容器
    // 这里对 CAxWindow 进行超类化，使得 BrowserView 类也能够作为 ActiveX 控件的容器
    DECLARE_WND_SUPERCLASS(IEBROWSER_WINDOW_CLASS_NAME, CAxWindow::GetWndClassName());

private:
    // 以下实现 IEBrowser 窗口本身的事件响应

    BEGIN_MSG_MAP(IEBrowser)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

    ///
    // WM_CREATE
    ///
    LRESULT OnCreate(unsigned int message, WPARAM w_param, LPARAM l_param, BOOL& is_handled);

    ///
    // WM_DESTORY
    ///
    LRESULT OnDestroy(unsigned int message, WPARAM w_param, LPARAM l_param, BOOL& is_handled);

public:
    // 以下实现 DIID_DWebBrowserEvents2 中的事件响应

    BEGIN_SINK_MAP(IEBrowser)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
    END_SINK_MAP()

    ///
    // 导航动作发生前触发
    ///
    void __stdcall OnBeforeNavigate2(
        IDispatch* dispatch,
        VARIANT* url,
        VARIANT* flags,
        VARIANT* target_frame_name,
        VARIANT* post_data,
        VARIANT* headers,
        VARIANT_BOOL* cancel);

private:
    // 以下是私有函数

    ///
    // 创建容器窗口
    ///
    bool CreateBrowserWindow(HWND parent_window_handle);

    ///
    // 创建 WebBrowser 控件
    ///
    bool CreateWebBrowser();

    ///
    // 更新窗口的可见状态
    ///
    void UpdateVisible();

    ///
    // 更新窗口的大小
    ///
    void UpdateSize();

private:
    // 以下是成员变量

    // 委托处理器
    Delegate* delegate_;

    // WebBrowser 控件的 IWebBrowser2 接口指针
    CComPtr<IWebBrowser2> web_browser_;
};