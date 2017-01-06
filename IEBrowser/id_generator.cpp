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
        // 优先分配回收站的 id
        if (recycle_ids_.size() > 0)
        {
            id = recycle_ids_.front();
            recycle_ids_.pop_front();

            break;
        }

        // 回收站为空则从 id_num_ 递增生成新的 id
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

        // 检查是否已经回收过
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
