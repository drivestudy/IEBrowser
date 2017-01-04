#include "stdafx.h"

CComModule _Module;

void Run(HINSTANCE instance, const wchar_t* command_line, int command_show)
{
    // ...
}

int APIENTRY wWinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    LPWSTR command_line,
    int command_show)
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    _Module.Init(NULL, instance);

    Run(instance, command_line, command_show);

    _Module.Term();
    ::CoUninitialize();

    return 0;
}