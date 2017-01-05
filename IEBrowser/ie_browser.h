/******************************************************************************
*
* @description: IE 浏览器类，对 WebBrowser 控件进行封装
*
* @author:      Dongyu
* @date:        2017.1.4
*
******************************************************************************/

#pragma once

#define IEBROWSER_WINDOW_CLASS_NAME L"IEBrowserWindowClass"
#define IEBROWSER_WINDOW_NAME L"IEBrowserWindow"

///
// IE 浏览器类，封装 WebBrowser 控件
///
class IEBrowser :
    public CWindowImpl<IEBrowser, CAxWindow>
{
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
    // 导航一个 url
    ///
    bool Navigate(const wchar_t* url);

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

    // WebBrowser 控件的 IWebBrowser2 接口指针
    CComPtr<IWebBrowser2> web_browser_;
};