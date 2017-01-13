#include "stdafx.h"

#include <crtdbg.h>

#include "ipc_server.h"

CComModule _Module;

void Run(HINSTANCE instance, const wchar_t* command_line, int command_show)
{
    IPCServer* ipc_server = IPCServer::GetInstance();
    ipc_server->Start(L"E15B9373-6737-4EC6-B461-E5BCFE0134D3");
    ipc_server->Stop();
}

int APIENTRY wWinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    LPWSTR command_line,
    int command_show)
{
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    _Module.Init(NULL, instance);

    Run(instance, command_line, command_show);

    _Module.Term();

    return 0;
}