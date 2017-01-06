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