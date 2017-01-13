#pragma once

///
// ���Դ洢����ֵ
///
class IPCValue
{
public:
    ///
    // ֵ������
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

    // ��������ֵ
    void Swap(IPCValue& another);

    // ��ȡ����
    Type GetType() const;

    // ��ȡֵ
    bool GetBool() const;
    int GetInt() const;
    double GetDouble() const;
    unsigned int GetUInt() const;
    unsigned long GetULong() const;
    const wchar_t* GetString() const;

    // ��ȡ��ǰֵռ�õ��ڴ�
    size_t GetValueSize() const;

private:
    // ֵ������
    Type type_;

    // �洢�����ֵ
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
