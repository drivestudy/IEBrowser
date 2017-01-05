#include "stdafx.h"
#include "com_utility.h"

bool ComUtility::VTToBool(VARIANT * var, bool default_ret)
{
    bool result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_BOOL)
        {
            break;
        }

        result = var->boolVal == VARIANT_TRUE;

        break;
    }

    return result;
}

short ComUtility::VTToShort(VARIANT * var, short default_ret)
{
    short result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_I2)
        {
            break;
        }

        result = var->iVal;

        break;
    }

    return result;
}

int ComUtility::VTToInt(VARIANT * var, int default_ret)
{
    int result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_INT)
        {
            break;
        }

        result = var->intVal;

        break;
    }

    return result;
}

unsigned int ComUtility::VTToUInt(VARIANT * var, unsigned int default_ret)
{
    unsigned int result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_UINT)
        {
            break;
        }

        result = var->uintVal;

        break;
    }

    return result;
}

long ComUtility::VTToLong(VARIANT * var, long default_ret)
{
    long result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_I4)
        {
            break;
        }

        result = var->lVal;

        break;
    }

    return result;
}

unsigned long ComUtility::VTToULong(VARIANT * var, unsigned long default_ret)
{
    long result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_UI4)
        {
            break;
        }

        result = var->ulVal;

        break;
    }

    return result;
}

float ComUtility::VTToFloat(VARIANT * var, float default_ret)
{
    float result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_R4)
        {
            break;
        }

        result = var->fltVal;

        break;
    }

    return result;
}

double ComUtility::VTToDouble(VARIANT * var, double default_ret)
{
    double result = default_ret;

    for (;;)
    {
        if (var == nullptr)
        {
            break;
        }

        if (var->vt != VT_R8)
        {
            break;
        }

        result = var->dblVal;

        break;
    }

    return result;
}

const wchar_t * ComUtility::VTToString(VARIANT * var, const wchar_t * default_ret)
{
    if (var == nullptr || var->vt != VT_BSTR)
    {
        return default_ret;
    }
    else
    {
        return var->bstrVal;
    }
}

IDispatch * ComUtility::VTToIDispatch(VARIANT * var)
{
    if (var == nullptr || var->vt != VT_DISPATCH)
    {
        return nullptr;
    }
    else
    {
        return var->pdispVal;
    }
}