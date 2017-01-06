#include "stdafx.h"
#include "ie_browser_manager.h"

#include "ie_browser_setting.h"
#include "ie_browser_thread.h"

IEBrowserManager::IEBrowserManager()
{
}

IEBrowserManager::~IEBrowserManager()
{
}

IEBrowserManager * IEBrowserManager::GetInstance()
{
    static IEBrowserManager instance;
    return &instance;
}

bool IEBrowserManager::CreateBrowser(IEBrowserSetting & setting, unsigned int & browser_id)
{
    bool result = false;

    do
    {
        browser_id = id_generator_.Generate();
        if (browser_id == IdGenerator::INVALID_ID)
        {
            break;
        }

        // 创建 IEBrowserThread
        std::shared_ptr<IEBrowserThread> browser_thread(new IEBrowserThread);
        if (!browser_thread->Initialize(setting))
        {
            break;
        }

        // 记录 IEBrowserThread
        browser_thread_map_.insert({ browser_id, browser_thread });

        result = true;

    } while (false);

    if (!result)
    {
        id_generator_.Recycle(browser_id);
    }

    return result;
}

bool IEBrowserManager::DestroyBrowser(unsigned int browser_id)
{
    bool result = false;

    do
    {
        if (browser_id == IdGenerator::INVALID_ID)
        {
            break;
        }

        auto iter = browser_thread_map_.find(browser_id);
        if (iter == browser_thread_map_.end())
        {
            break;
        }

        // 反初始化 IEBrowserThread 对象
        std::shared_ptr<IEBrowserThread> browser_thread = iter->second;
        browser_thread->UnInitialize();

        // 移除 IEBrowserThread 对象
        browser_thread_map_.erase(iter);

        // 回收 id
        id_generator_.Recycle(browser_id);

        result = true;

    } while (false);

    return result;
}
