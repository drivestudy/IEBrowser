/******************************************************************************
*
* @description: IEBrowser ������������װ�˴��ݸ� IEBrowser::Initialize() �Ĳ���
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

///
// ��ʼ�� IEBrowser ʱ��������
///
struct IEBrowserSetting
{
    IEBrowserSetting() :
        parent_window_handle_(nullptr)
    {
    }

    HWND parent_window_handle_;
};
