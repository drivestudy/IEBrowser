/******************************************************************************
*
* @description: Id �����������ڲ��� id, ֻ��֤������ id ���ظ�
*
* @author:      Dongyu
* @date:        2017.1.7
*
******************************************************************************/

#pragma once

///
// Id ������
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
    // ����һ���µ� id
    ///
    unsigned int Generate();

    ///
    // ���ղ����˵� id
    ///
    void Recycle(unsigned int id);

private:
    // ��ǰ id ֵ���䵽�˵ڼ�λ
    unsigned int id_num_;

    // ���ջ����� id
    std::deque<unsigned int> recycle_ids_;
};