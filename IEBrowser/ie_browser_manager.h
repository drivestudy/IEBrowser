/******************************************************************************
*
* @description: IE ����������ߣ����� IEBrowserThread �Ĵ���������
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
// IEBrowserThread �Ĺ����ߣ����� IEBrowserThread �Ĵ���������
// ��ֱ��ά�� IEBrowser ���󣬶���ά�� IEBrowserThread ����
// ��һ������
///
class IEBrowserManager
{
private:
    IEBrowserManager();
    ~IEBrowserManager();

public:
    // �����Ƕ���ӿ�

    ///
    // ��ȡ����
    ///
    static IEBrowserManager* GetInstance();

    ///
    // ����һ�������
    ///
    bool CreateBrowser(IEBrowserSetting& setting, unsigned int& browser_id);

    ///
    // ����һ�������
    ///
    bool DestroyBrowser(unsigned int browser_id);

private:
    // �����ǳ�Ա����

    // id ��������Ϊ IEBrowserThread �������� id
    IdGenerator id_generator_;

    // �Ѵ����� IEBrowserThread �б�
    // key �� browser_id, value �� IEBrowserThread ����
    std::map<unsigned int, std::shared_ptr<IEBrowserThread>> browser_thread_map_;
};