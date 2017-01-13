#pragma once

#include <deque>

#include "ipc_value.h"

///
// ���ڽ��̼�ͨ��ʱ��������
// ��� buffer ��һ���б�����԰�Ҫ���ݵ�������� push ���б���
///
class IPCBuffer
{
public:
    IPCBuffer();
    ~IPCBuffer();

    ///
    // ���洢��ֵ���л�Ϊһ���ڴ�
    // ʹ������Ҫ�����ͷ�����ڴ�
    ///
    bool Encode(char*& buffer, size_t& buffer_size) const;

    ///
    // ��һ���ڴ��з����л��� IPCBuffer
    ///
    bool Decode(char* buffer, size_t buffer_size);

    ///
    // ��һ��ֵ��ӵ��б�ĩβ
    ///
    void Push(IPCValue value);

    ///
    // ���б�ͷȡ��һ��ֵ
    ///
    IPCValue Take();

private:
    // ����ֵ���б�
    std::deque<IPCValue> values_;
};
