#include "stdafx.h"
#include "id_generator.h"

IdGenerator::IdGenerator() :
    id_num_(MINIMUM_ID)
{
}

IdGenerator::~IdGenerator()
{
}

unsigned int IdGenerator::Generate()
{
    unsigned int id = INVALID_ID;

    do
    {
        // ���ȷ������վ�� id
        if (recycle_ids_.size() > 0)
        {
            id = recycle_ids_.front();
            recycle_ids_.pop_front();

            break;
        }

        // ����վΪ����� id_num_ ���������µ� id
        if (id_num_ == MAXIMUM_ID)
        {
            break;
        }

        id = id_num_;

        id_num_++;

    } while (false);

    return id;
}

void IdGenerator::Recycle(unsigned int id)
{
    do
    {
        if (id == INVALID_ID)
        {
            break;
        }

        if (id < MINIMUM_ID || id > MAXIMUM_ID)
        {
            break;
        }

        // ����Ƿ��Ѿ����չ�
        bool is_recycled = false;
        for (auto recycled_id : recycle_ids_)
        {
            if (id == recycled_id)
            {
                is_recycled = true;
                break;
            }
        }

        if (is_recycled)
        {
            break;
        }

        recycle_ids_.push_back(id);

    } while (false);
}
