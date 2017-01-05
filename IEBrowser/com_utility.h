#pragma once

namespace ComUtility
{
    ///
    // Variant תΪ bool
    // default_ret ��ת��ʧ��ʱ��Ĭ��ֵ
    ///
    bool VTToBool(VARIANT* var, bool default_ret = false);

    ///
    // Variant תΪ short
    ///
    short VTToShort(VARIANT* var, short default_ret = 0);

    ///
    // Variant תΪ int
    ///
    int VTToInt(VARIANT* var, int default_ret = 0);

    ///
    // VARIANT תΪ unsigned int
    ///
    unsigned int VTToUInt(VARIANT* var, unsigned int default_ret = 0);

    ///
    // VARIANT תΪ long
    ///
    long VTToLong(VARIANT* var, long default_ret = 0);

    ///
    // VARIANT תΪ unsigned long
    ///
    unsigned long VTToULong(VARIANT* var, unsigned long default_ret = 0);

    ///
    // VARIANT תΪ float
    ///
    float VTToFloat(VARIANT* var, float default_ret = 0.0);

    ///
    // VARIANT תΪ double
    ///
    double VTToDouble(VARIANT* var, double default_ret = 0.0);

    ///
    // VARIANT תΪ const wchar_t*
    ///
    const wchar_t* VTToString(VARIANT* var, const wchar_t* default_ret = nullptr);

    ///
    // VARIANT תΪ IDispatch*
    ///
    IDispatch* VTToIDispatch(VARIANT* var);
}
