# IEBrowser

## Bug

- 如果为 `IEBrowserThread::Initialize()` 传入父窗口句柄(在主线程中创建)，就会卡死。
    - 现在尚不清楚卡死的原因，应该与父子窗口关系有关，想到的解决方案是创建窗口的时候先不要设置父窗口句柄，等窗口创建完毕后再设置。

## TODO

- 编写 BrowserManager 类，用来管理 IEBrowserThread