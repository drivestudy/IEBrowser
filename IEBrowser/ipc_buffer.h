#pragma once

#include <deque>

#include "ipc_value.h"

///
// 用于进程间通信时传递数据
// 这个 buffer 是一个列表，你可以把要传递的数据逐个 push 到列表中
///
class IPCBuffer
{
public:
    IPCBuffer();
    ~IPCBuffer();

    ///
    // 将存储的值序列化为一块内存
    // 使用者需要自行释放这块内存
    ///
    bool Encode(char*& buffer, size_t& buffer_size) const;

    ///
    // 从一块内存中反序列化出 IPCBuffer
    ///
    bool Decode(char* buffer, size_t buffer_size);

    ///
    // 将一个值添加到列表末尾
    ///
    void Push(IPCValue value);

    ///
    // 从列表开头取出一个值
    ///
    IPCValue Take();

private:
    // 所有值的列表
    std::deque<IPCValue> values_;
};
