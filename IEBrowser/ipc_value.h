#pragma once

///
// 可以存储各种值
///
class IPCValue
{
public:
    ///
    // 值的类型
    ///
    enum Type
    {
        VT_NULL,
        VT_BOOL,
        VT_INT,
        VT_DOUBLE,
        VT_UINT,
        VT_ULONG,
        VT_STRING
    };

    IPCValue();
    explicit IPCValue(bool value);
    explicit IPCValue(int value);
    explicit IPCValue(double value);
    explicit IPCValue(unsigned int value);
    explicit IPCValue(unsigned long value);
    explicit IPCValue(const wchar_t* str);
    IPCValue(const IPCValue& another);
    IPCValue(IPCValue&& another);
    IPCValue& operator = (IPCValue another);
    ~IPCValue();

    // 交换两个值
    void Swap(IPCValue& another);

    // 获取类型
    Type GetType() const;

    // 获取值
    bool GetBool() const;
    int GetInt() const;
    double GetDouble() const;
    unsigned int GetUInt() const;
    unsigned long GetULong() const;
    const wchar_t* GetString() const;

    // 获取当前值占用的内存
    size_t GetValueSize() const;

private:
    // 值的类型
    Type type_;

    // 存储具体的值
    union
    {
        bool bool_value_;
        int int_value_;
        double double_value_;
        unsigned int uint_value_;
        unsigned long ulong_value_;
        wchar_t* string_value_;
    } value_;
};
