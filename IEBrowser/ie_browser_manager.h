/******************************************************************************
*
* @description: IE 浏览器管理者，负责 IEBrowserThread 的创建和销毁
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

#include "id_generator.h"

struct IEBrowserSetting;
class IEBrowserThread;

///
// IEBrowserThread 的管理者，负责 IEBrowserThread 的创建和销毁
// 不直接维护 IEBrowser 对象，而是维护 IEBrowserThread 对象
// 是一个单例
///
class IEBrowserManager
{
private:
    IEBrowserManager();
    ~IEBrowserManager();

public:
    // 以下是对外接口

    ///
    // 获取单例
    ///
    static IEBrowserManager* GetInstance();

    ///
    // 创建一个浏览器
    ///
    bool CreateBrowser(IEBrowserSetting& setting, unsigned int& browser_id);

    ///
    // 销毁一个浏览器
    ///
    bool DestroyBrowser(unsigned int browser_id);

private:
    // 以下是成员变量

    // id 生成器，为 IEBrowserThread 对象生成 id
    IdGenerator id_generator_;

    // 已创建的 IEBrowserThread 列表
    // key 是 browser_id, value 是 IEBrowserThread 对象
    std::map<unsigned int, std::shared_ptr<IEBrowserThread>> browser_thread_map_;
};