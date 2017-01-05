/******************************************************************************
*
* @description: IE ������࣬�� WebBrowser �ؼ����з�װ
*
* @author:      Dongyu
* @date:        2017.1.4
*
******************************************************************************/

#pragma once

#define IEBROWSER_WINDOW_CLASS_NAME L"IEBrowserWindowClass"
#define IEBROWSER_WINDOW_NAME L"IEBrowserWindow"

///
// IE ������࣬��װ WebBrowser �ؼ�
///
class IEBrowser :
    public CWindowImpl<IEBrowser, CAxWindow>
{
public:
    IEBrowser();
    ~IEBrowser();

    ///
    // ��ʼ��
    ///
    bool Initialize(HWND parent_window_handle);

    ///
    // ����ʼ��
    ///
    void UnInitialze();

    ///
    // ����һ�� url
    ///
    bool Navigate(const wchar_t* url);

public:
    // ���໯ CAxWindow, CAxWindow ������Ϊ ActiveX �ؼ�������
    // ����� CAxWindow ���г��໯��ʹ�� BrowserView ��Ҳ�ܹ���Ϊ ActiveX �ؼ�������
    DECLARE_WND_SUPERCLASS(IEBROWSER_WINDOW_CLASS_NAME, CAxWindow::GetWndClassName());

private:
    // ����ʵ�� IEBrowser ���ڱ�����¼���Ӧ

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
    // ������˽�к���

    ///
    // ������������
    ///
    bool CreateBrowserWindow(HWND parent_window_handle);

    ///
    // ���� WebBrowser �ؼ�
    ///
    bool CreateWebBrowser();

    ///
    // ���´��ڵĿɼ�״̬
    ///
    void UpdateVisible();

    ///
    // ���´��ڵĴ�С
    ///
    void UpdateSize();

private:
    // �����ǳ�Ա����

    // WebBrowser �ؼ��� IWebBrowser2 �ӿ�ָ��
    CComPtr<IWebBrowser2> web_browser_;
};