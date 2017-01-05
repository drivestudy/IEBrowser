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
    // ���´��ڵĿɼ�״̬
    ///
    void UpdateVisible();

    ///
    // ���´��ڵĴ�С
    ///
    void UpdateSize();
};