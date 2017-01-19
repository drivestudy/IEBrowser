#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

#include <WinBase.h>

#ifdef _DEBUG

#define LOG_BUFFER_SIZE 4096

#define LOG_LEVEL_INFO L"[log][info]"
#define LOG_LEVEL_WARNING L"[log][warning]"
#define LOG_LEVEL_ERROR L"[log][warning]"
#define LOG_LEVEL_CRITICAL L"[log][critical]"
#define LOG_LEVEL_VERBOSE L"[log][verbose]"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)

inline void LogMessage(
    const wchar_t* level, 
    const wchar_t* file, 
    const wchar_t* function, 
    unsigned int line, 
    const wchar_t* format,
    ...)
{
    std::wstring format_str = level;
    format_str += L" ";
    format_str += function;
    format_str += L" ";
    format_str += format;

    format = format_str.c_str();

    wchar_t buffer[LOG_BUFFER_SIZE] = { 0 };

    va_list args = nullptr;
    va_start(args, format);
    _vsnwprintf_s(buffer, LOG_BUFFER_SIZE, _TRUNCATE, format, args);
    va_end(args);

    OutputDebugStringW(buffer);
}

#define LOG_INFO(format, ...)   \
{                               \
    LogMessage(LOG_LEVEL_INFO, __WFILE__, __WFUNCTION__, __LINE__, format, __VA_ARGS__);  \
}

#define LOG_WARNING(format, ...)   \
{                               \
    LogMessage(LOG_LEVEL_WARNING, __WFILE__, __WFUNCTION__, __LINE__, format, __VA_ARGS__);  \
}

#define LOG_ERROR(format, ...)   \
{                               \
    LogMessage(LOG_LEVEL_ERROR, __WFILE__, __WFUNCTION__, __LINE__, format, __VA_ARGS__);  \
}

#define LOG_CRITICAL(format, ...)   \
{                               \
    LogMessage(LOG_LEVEL_CRITICAL, __WFILE__, __WFUNCTION__, __LINE__, format, __VA_ARGS__);  \
}

#define LOG_VERBOSE(format, ...)   \
{                               \
    LogMessage(LOG_LEVEL_VERBOSE, __WFILE__, __WFUNCTION__, __LINE__, format, __VA_ARGS__);  \
}

#define LOG_ENTER() LOG_INFO(L"Enter")
#define LOG_EXIT() LOG_INFO(L"Exit")

#define LOG_ENTER_EX(format, ...)  \
{   \
    LogMessage(LOG_LEVEL_INFO, __WFILE__, __WFUNCTION__, __LINE__, L"Enter " format, __VA_ARGS__);  \
}

#define LOG_EXIT_EX(format, ...) \
{   \
    LogMessage(LOG_LEVEL_INFO, __WFILE__, __WFUNCTION__, __LINE__, L"Exit " format, __VA_ARGS__);  \
}

#else

#define LOG_INFO(x, ...) 
#define LOG_WARNING(x, ...) 
#define LOG_ERROR(x, ...) 
#define LOG_CRITICAL(x, ...) 
#define LOG_VERBOSE(x, ...) 

#define LOG_ENTER()
#define LOG_EXIT()
#define LOG_ENTER_EX(x, ...)
#define LOG_EXIT_EX(x, ...)

#endif // _DEBUG
