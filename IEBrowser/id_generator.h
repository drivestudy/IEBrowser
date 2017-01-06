/******************************************************************************
*
* @description: Id 生成器，用于产生 id, 只保证产生的 id 不重复
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

///
// Id 生成器
///
class IdGenerator
{
public:
    enum IdDefine
    {
        INVALID_ID = 0,
        MINIMUM_ID = 1,
        MAXIMUM_ID = UINT_MAX,
    };

public:
    IdGenerator();
    ~IdGenerator();

    ///
    // 生成一个新的 id
    ///
    unsigned int Generate();

    ///
    // 回收不用了的 id
    ///
    void Recycle(unsigned int id);

private:
    // 当前 id 值分配到了第几位
    unsigned int id_num_;

    // 回收回来的 id
    std::deque<unsigned int> recycle_ids_;
};