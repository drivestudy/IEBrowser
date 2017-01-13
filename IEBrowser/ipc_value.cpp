#include "stdafx.h"
#include "ipc_value.h"

#include <cstring>
#include <utility>

IPCValue::IPCValue() :
    type_(VT_NULL)
{
    value_.string_value_ = nullptr;
}

IPCValue::IPCValue(bool value) :
    type_(VT_BOOL)
{
    value_.bool_value_ = value;
}

IPCValue::IPCValue(int value) :
    type_(VT_INT)
{
    value_.int_value_ = value;
}

IPCValue::IPCValue(double value) :
    type_(VT_DOUBLE)
{
    value_.double_value_ = value;
}

IPCValue::IPCValue(unsigned int value) :
    type_(VT_UINT)
{
    value_.uint_value_ = value;
}

IPCValue::IPCValue(unsigned long value) :
    type_(VT_ULONG)
{
    value_.ulong_value_ = value;
}

IPCValue::IPCValue(const wchar_t * str)
{
    if (str == nullptr)
    {
        type_ = VT_NULL;
        value_.string_value_ = nullptr;
    }
    else
    {
        type_ = VT_STRING;
        size_t length = wcslen(str);
        value_.string_value_ = new wchar_t[length + 1];
        wcscpy_s(value_.string_value_, length + 1, str);
    }
}

IPCValue::IPCValue(const IPCValue& another)
{
    type_ = another.type_;

    if (type_ == VT_STRING)
    {
        size_t length = wcslen(another.GetString());
        value_.string_value_ = new wchar_t[length + 1];
        wcscpy_s(value_.string_value_, length + 1, another.GetString());
    }
    else
    {
        value_ = another.value_;
    }
}

IPCValue::IPCValue(IPCValue && another)
{
    type_ = another.type_;
    value_ = another.value_;

    another.type_ = VT_NULL;
    another.value_.string_value_ = nullptr;
}

IPCValue & IPCValue::operator=(IPCValue another)
{
    Swap(another);
    return *this;
}

IPCValue::~IPCValue()
{
    if (type_ == VT_STRING && value_.string_value_)
    {
        delete[] value_.string_value_;
    }
}

void IPCValue::Swap(IPCValue & another)
{
    std::swap(type_, another.type_);
    std::swap(value_, another.value_);
}

IPCValue::Type IPCValue::GetType() const
{
    return type_;
}

bool IPCValue::GetBool() const
{
    return value_.bool_value_;
}

int IPCValue::GetInt() const
{
    return value_.int_value_;
}

double IPCValue::GetDouble() const
{
    return value_.double_value_;
}

unsigned int IPCValue::GetUInt() const
{
    return value_.uint_value_;
}

unsigned long IPCValue::GetULong() const
{
    return value_.ulong_value_;
}

const wchar_t * IPCValue::GetString() const
{
    return value_.string_value_;
}

size_t IPCValue::GetValueSize() const
{
    size_t size = 0;

    switch (type_)
    {
    case IPCValue::VT_NULL:
        size = 0;
        break;
    case IPCValue::VT_BOOL:
        size = sizeof(bool);
        break;
    case IPCValue::VT_INT:
        size = sizeof(int);
        break;
    case IPCValue::VT_DOUBLE:
        size = sizeof(double);
        break;
    case IPCValue::VT_UINT:
        size = sizeof(unsigned int);
        break;
    case IPCValue::VT_ULONG:
        size = sizeof(unsigned long);
        break;
    case IPCValue::VT_STRING:
        size = (wcslen(value_.string_value_) + 1) * sizeof(wchar_t);
        break;
    default:
        break;
    }

    return size;
}
