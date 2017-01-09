#include "stdafx.h"
#include "ipc_server.h"

IPCServer::IPCServer()
{
}

IPCServer::~IPCServer()
{
}

IPCServer * IPCServer::GetInstance()
{
    IPCServer instance;
    return &instance;
}

bool IPCServer::Start()
{
    do
    {

    } while (false);

    return false;
}

void IPCServer::Stop()
{
}

void IPCServer::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

void IPCServer::RemoveDelegate()
{
    delegate_ = nullptr;
}

bool IPCServer::SendCommand(
    unsigned int client_id, 
    unsigned int command_id, 
    void * data, 
    size_t data_size, 
    unsigned int time_out)
{
    return false;
}

bool IPCServer::PostCommand(
    unsigned int client_id, 
    unsigned int command_id, 
    void * data, 
    size_t data_size)
{
    return false;
}
