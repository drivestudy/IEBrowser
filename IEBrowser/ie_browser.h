/******************************************************************************
*
* @description: IE ������࣬�� WebBrowser �ؼ����з�װ
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
// IE ������࣬��װ WebBrowser �ؼ�
///
class IEBrowser :
    public CWindowImpl<IEBrowser, CAxWindow>,
    public IDispEventImpl<IEBROWSER_DISP_ID, IEBrowser, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 0>
{
public:
    ///
    // ί���࣬ʹ���߼̳��������Լ���������¼�
    ///
    class Delegate
    {
    public:
        ///
        // ����ǰ�������¼�
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
    // ��ʼ��
    ///
    bool Initialize(HWND parent_window_handle);

    ///
    // ����ʼ��
    ///
    void UnInitialze();

    ///
    // ���ί�д�����
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // �Ƴ�ί�д�����
    ///
    void RemoveDelegate();

    ///
    // ����һ�� url
    ///
    bool Navigate(const wchar_t* url);

    ///
    // ֹͣ���ڽ��еĵ�������
    ///
    bool Stop();

    ///
    // ˢ�µ�ǰҳ��
    ///
    bool Refresh();

    ///
    // ǰ������
    ///
    bool GoForward();

    ///
    // ���˲���
    ///
    bool GoBack();

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

public:
    // ����ʵ�� DIID_DWebBrowserEvents2 �е��¼���Ӧ

    BEGIN_SINK_MAP(IEBrowser)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
    END_SINK_MAP()

    ///
    // ������������ǰ����
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

    // ί�д�����
    Delegate* delegate_;

    // WebBrowser �ؼ��� IWebBrowser2 �ӿ�ָ��
    CComPtr<IWebBrowser2> web_browser_;
};