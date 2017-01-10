#include "stdafx.h"
#include "ipc_client.h"

IPCClient::IPCClient() :
    delegate_(nullptr)
{
}

IPCClient::~IPCClient()
{
}

IPCClient * IPCClient::GetInstance()
{
    static IPCClient instance;
    return &instance;
}

bool IPCClient::Connect()
{
    return false;
}

void IPCClient::DisConnect()
{
}

void IPCClient::SetDelegate(Delegate * delegate)
{
    delegate_ = delegate;
}

void IPCClient::RemoveDelegate()
{
    delegate_ = nullptr;
}

bool IPCClient::SendCommand(
    unsigned int command_id, 
    void * data, 
    size_t data_size, 
    unsigned int time_out)
{
    return false;
}

bool IPCClient::PostCommand(unsigned int command_id, void * data, size_t data_size)
{
    return false;
}
