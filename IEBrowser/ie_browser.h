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

struct IEBrowserSetting;

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
            bool is_main_frame,
            const wchar_t* url, 
            const wchar_t* target_frame_name, 
            const wchar_t* post_data, 
            const wchar_t* headers) {};

        ///
        // ��������¼�
        ///
        virtual void OnNativeComplete(bool is_main_frame, const wchar_t* url) {};

        ///
        // ���������¼�
        ///
        virtual void OnNavigateError(
            bool is_main_frame, 
            const wchar_t* url, 
            const wchar_t* target_frame_name, 
            long status_code) {};

        ///
        // �ĵ���������¼�
        ///
        virtual void OnDocumentComplete(bool is_main_frame, const wchar_t* url) {};

        ///
        // ����ı��¼�
        ///
        virtual void OnTitleChange(const wchar_t* title) {};

        ///
        // status text �ı��¼�
        ///
        virtual void OnStatusTextChange(const wchar_t* status_text) {};

        ///
        // command state �ı��¼�
        ///
        virtual void OnCommandStateChange(
            bool can_refresh, 
            bool can_go_forward, 
            bool can_go_back) {};

        ///
        // ���ȸı��¼�
        ///
        virtual void OnProgressChange(long progress, long max_progress) {};
    };

public:
    IEBrowser();
    ~IEBrowser();

    ///
    // ��ʼ��
    ///
    bool Initialize(IEBrowserSetting& setting);

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
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR, OnNavigateError)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_TITLECHANGE, OnTitleChange)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_STATUSTEXTCHANGE, OnStatusTextChange)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_COMMANDSTATECHANGE, OnCommandStateChange)
        SINK_ENTRY_EX(IEBROWSER_DISP_ID, DIID_DWebBrowserEvents2, DISPID_PROGRESSCHANGE, OnProgressChange)
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

    ///
    // ������ɺ󴥷����¼�
    ///
    void __stdcall OnNavigateComplete2(IDispatch* dispatch, VARIANT* url);

    ///
    // ��������󴥷����¼�
    ///
    void __stdcall OnNavigateError(
        IDispatch* dispatch,
        VARIANT* url,
        VARIANT* target_frame_name,
        VARIANT* status_code,
        VARIANT_BOOL* cancel);

    ///
    // ҳ�������ɺ󴥷����¼�
    ///
    void __stdcall OnDocumentComplete(IDispatch* dispatch, VARIANT* url);

    ///
    // ҳ�����ı�ʱ�������¼�
    ///
    void __stdcall OnTitleChange(BSTR title);

    ///
    // status text �ı�ʱ�������¼�
    ///
    void __stdcall OnStatusTextChange(BSTR status_text);

    ///
    // command status �ı�ʱ�������¼�
    ///
    void __stdcall OnCommandStateChange(long command, VARIANT_BOOL enable);

    ///
    // ���ȸı�ʱ�������¼�
    ///
    void __stdcall OnProgressChange(long progress, long max_progress);

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

    ///
    // �ж� dispatch �Ƿ��� main frame
    ///
    bool IsMainFrame(IDispatch* dispatch);

private:
    // �����ǳ�Ա����

    // ί�д�����
    Delegate* delegate_;

    // WebBrowser �ؼ��� IWebBrowser2 �ӿ�ָ��
    CComPtr<IWebBrowser2> web_browser_;
};