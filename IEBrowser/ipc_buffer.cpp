#include "stdafx.h"
#include "ipc_buffer.h"

IPCBuffer::IPCBuffer()
{
}

IPCBuffer::~IPCBuffer()
{
}

bool IPCBuffer::Encode(char *& buffer, size_t & buffer_size) const
{
    // �������
    // 
    // NULL:
    // |__type__|
    // �ڴ濪ʼ�� type, ���ֽ�)
    // 
    // ��������(bool, int, boolean):
    // |__type__|__value__| 
    // �ڴ濪ʼ�� type, ���ֽ�), ������� value_.bool_value_(sizeof(bool)) �ȡ�
    //
    // �ַ���
    // |__type__|__size__|__string__| 
    // �ڴ濪ʼ�� type, ���ֽ�), ��������ַ����� byte ����(size_t, ���ֽ�)��������β�� \0
    // ���Ž����ַ�����������β�� \0

    bool result = false;
    buffer = nullptr;
    buffer_size = 0;

    do
    {
        if (values_.empty())
        {
            LOG_ERROR(L"values_ is empty");
            break;
        }

        // �ȼ������Ҫ���ڴ��С��Ȼ���ٿ�ʼ����
        buffer_size = 0;
        for (auto& value : values_)
        {
            // type �̶�ռ 4 ���ֽ�
            buffer_size += 4;

            buffer_size += value.GetValueSize();

            // string ���� 4 �ֽ�ר�ż�¼�ַ�����ռ�ڴ�
            if (value.GetType() == IPCValue::VT_STRING)
            {
                buffer_size += 4;
            }
        }

        // �����ڴ�
        buffer = new char[buffer_size];

        // ��ʼ����
        size_t pos = 0;
        for (auto& value : values_)
        {
            IPCValue::Type type = value.GetType();
            memcpy(&(buffer[pos]), &type, sizeof(type));
            pos += sizeof(type);

            switch (value.GetType())
            {
            case IPCValue::VT_NULL:
                break;
            case IPCValue::VT_BOOL:
            {
                bool var = value.GetBool();
                memcpy(&(buffer[pos]), &var, sizeof(var));
                pos += sizeof(var);

                break;
            }
            case IPCValue::VT_INT:
            {
                int var = value.GetInt();
                memcpy(&(buffer[pos]), &var, sizeof(var));
                pos += sizeof(var);

                break;
            }
            case IPCValue::VT_DOUBLE:
            {
                double var = value.GetDouble();
                memcpy(&(buffer[pos]), &var, sizeof(var));
                pos += sizeof(var);

                break;
            }
            case IPCValue::VT_UINT:
            {
                unsigned int var = value.GetUInt();
                memcpy(&(buffer[pos]), &var, sizeof(var));
                pos += sizeof(var);

                break;
            }
            case IPCValue::VT_ULONG:
            {
                unsigned long var = value.GetULong();
                memcpy(&(buffer[pos]), &var, sizeof(var));
                pos += sizeof(var);

                break;
            }
            case IPCValue::VT_STRING:
            {
                const wchar_t* str = value.GetString();
                size_t size = value.GetValueSize();
                memcpy(&(buffer[pos]), &size, sizeof(size));
                pos += sizeof(size);

                memcpy(&(buffer[pos]), str, size);
                pos += size;

                break;
            }
            default:
                break;
            }
        }

        result = true;

    } while (false);

    if (!result && buffer)
    {
        delete[] buffer;
    }

    return result;
}

bool IPCBuffer::Decode(char * buffer, size_t buffer_size)
{
    bool result = false;

    do
    {
        if (buffer == nullptr || buffer_size == 0)
        {
            LOG_ERROR(L"buffer is not valid, buffer = 0x%08x, buffer_size = %d", buffer, buffer_size);
            break;
        }

        // �Ȱѽ���浽һ����ʱ���������ɹ�֮���ٽ������Ա��������ʱ��Ӱ��ԭ���洢��ֵ
        std::deque<IPCValue> temp_values_;

        bool no_error = true;
        size_t pos = 0;

        while (pos < buffer_size && no_error)
        {
            IPCValue::Type type;
            memcpy(&type, &(buffer[pos]), sizeof(type));
            pos += sizeof(type);

            switch (type)
            {
            case IPCValue::VT_NULL:
                temp_values_.push_back(IPCValue());
                break;
            case IPCValue::VT_BOOL:
            {
                bool var = false;
                memcpy(&var, &(buffer[pos]), sizeof(var));
                pos += sizeof(var);

                temp_values_.push_back(IPCValue(var));

                break;
            }
            case IPCValue::VT_INT:
            {
                int var = 0;
                memcpy(&var, &(buffer[pos]), sizeof(var));
                pos += sizeof(var);

                temp_values_.push_back(IPCValue(var));

                break;
            }
            case IPCValue::VT_DOUBLE:
            {
                double var = 0.0;
                memcpy(&var, &(buffer[pos]), sizeof(var));
                pos += sizeof(var);

                temp_values_.push_back(IPCValue(var));

                break;
            }
            case IPCValue::VT_UINT:
            {
                unsigned int var = 0;
                memcpy(&var, &(buffer[pos]), sizeof(var));
                pos += sizeof(var);

                temp_values_.push_back(IPCValue(var));

                break;
            }
            case IPCValue::VT_ULONG:
            {
                unsigned long var = 0;
                memcpy(&var, &(buffer[pos]), sizeof(var));
                pos += sizeof(var);

                temp_values_.push_back(IPCValue(var));

                break;
            }
            case IPCValue::VT_STRING:
            {
                size_t size = 0;
                memcpy(&size, &(buffer[pos]), sizeof(size));
                pos += sizeof(size);

                const wchar_t* str = (wchar_t*)&(buffer[pos]);
                pos += size;

                temp_values_.push_back(IPCValue(str));

                break;
            }
            default:
                LOG_ERROR(L"unknown type, type = %d", type);
                no_error = false;
                break;
            }
        }

        if (!no_error || pos > buffer_size)
        {
            LOG_ERROR(L"error occur, no_error = %d, pos = %d, buffer_size = %d", no_error, pos, buffer_size);
            break;
        }

        values_.swap(temp_values_);

        result = true;

    } while (false);

    return result;
}

void IPCBuffer::PushBack(IPCValue value)
{
    values_.push_back(std::move(value));
}

void IPCBuffer::PushFront(IPCValue value)
{
    values_.push_front(std::move(value));
}

IPCValue IPCBuffer::TakeFront()
{
    IPCValue value;
    
    if (!values_.empty())
    {
        value = values_.front();
        values_.pop_front();
    }
    return value;
}

IPCValue IPCBuffer::TakeBack()
{
    IPCValue value;

    if (!values_.empty())
    {
        value = values_.back();
        values_.pop_back();
    }

    return value;
}
