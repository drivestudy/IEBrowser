/******************************************************************************
*
* @description: IE 浏览器窗口事件
*               包含了 IEBrowser 和 IEBrowserThread 之间互相发送的自定义窗口消息
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

enum IEBrowserWindowMessage
{
    // 浏览器线程初始化完毕后，将发送这个消息
    WM_IEBROWSER_THREAD_INIT_FINISH = WM_USER + 1,

    // 要反初始化 浏览器线程时，需要向浏览器线程发送这个消息
    WM_IEBROWSER_THREAD_UNINIT,

    // 浏览器线程已经反初始化完毕
    WM_IEBROWSER_THREAD_UNINIT_FINISH,
};