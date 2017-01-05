#pragma once

namespace ComUtility
{
    ///
    // Variant 转为 bool
    // default_ret 是转换失败时的默认值
    ///
    bool VTToBool(VARIANT* var, bool default_ret = false);

    ///
    // Variant 转为 short
    ///
    short VTToShort(VARIANT* var, short default_ret = 0);

    ///
    // Variant 转为 int
    ///
    int VTToInt(VARIANT* var, int default_ret = 0);

    ///
    // VARIANT 转为 unsigned int
    ///
    unsigned int VTToUInt(VARIANT* var, unsigned int default_ret = 0);

    ///
    // VARIANT 转为 long
    ///
    long VTToLong(VARIANT* var, long default_ret = 0);

    ///
    // VARIANT 转为 unsigned long
    ///
    unsigned long VTToULong(VARIANT* var, unsigned long default_ret = 0);

    ///
    // VARIANT 转为 float
    ///
    float VTToFloat(VARIANT* var, float default_ret = 0.0);

    ///
    // VARIANT 转为 double
    ///
    double VTToDouble(VARIANT* var, double default_ret = 0.0);

    ///
    // VARIANT 转为 const wchar_t*
    ///
    const wchar_t* VTToString(VARIANT* var, const wchar_t* default_ret = nullptr);

    ///
    // VARIANT 转为 IDispatch*
    ///
    IDispatch* VTToIDispatch(VARIANT* var);
}
