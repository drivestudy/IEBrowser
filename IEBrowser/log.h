#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

#include <WinBase.h>

#define LOG_MESSAGE(prefix)                                             \
{                                                                       \
    std::wstring format_str = prefix;                                   \
    format_str += format;                                               \
    format = format_str.c_str();                                        \
                                                                        \
    static const int buffer_length = 4096;                              \
    wchar_t buffer[buffer_length] = { 0 };                              \
                                                                        \
    va_list args = nullptr;                                             \
    va_start(args, format);                                             \
    _vsnwprintf_s(buffer, buffer_length - 1, _TRUNCATE, format, args);  \
    va_end(args);                                                       \
                                                                        \
    OutputDebugStringW(buffer);                                         \
}

void LOG_INFO(const wchar_t* format, ...)
{
#ifdef _DEBUG

    LOG_MESSAGE(L"[Log][Info] ");

#endif // _DEBUG
}

void LOG_WARNING(const wchar_t* format, ...)
{
#ifdef _DEBUG

    LOG_MESSAGE(L"[Log][Warning] ");

#endif // _DEBUG
}

void LOG_ERROR(const wchar_t* format, ...)
{
#ifdef _DEBUG

    LOG_MESSAGE(L"[Log][Error] ");

#endif // _DEBUG
}

void LOG_CRITICAL(const wchar_t* format, ...)
{
#ifdef _DEBUG

    LOG_MESSAGE(L"[Log][Critical] ");

#endif // _DEBUG
}
