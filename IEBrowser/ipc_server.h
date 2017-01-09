#pragma once

///
// ����һ�Զ�Ľ��̼�ͨ�ţ���һ�� server ���̺Ͷ�� client ����ͨ�ţ�
// ��װ Server �����ϵ�ͨ���߼�
///
class IPCServer
{
public:
    class Delegate
    {
    public:
        ///
        // ��ĳ�� client ����������
        ///
        void OnClientConnected(unsigned int client_id) {};

        ///
        // ĳ�� client �� server �Ͽ�������
        ///
        void OnClientDisConnected(unsigned int client_id) {};

        ///
        // �յ����� client ������
        ///
        void OnReceiveClientCommand(
            unsigned int client_id,
            unsigned int command_id,
            void* data,
            size_t data_size) {};
    };

private:
    IPCServer();
    ~IPCServer();

public:
    ///
    // ��ȡ����
    ///
    static IPCServer* GetInstance();

    ///
    // ���� server
    ///
    bool Start();

    ///
    // �ر� server
    ///
    void Stop();

    ///
    // ����ί��
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // �Ƴ�ί��
    ///
    void RemoveDelegate();

    ///
    // ��ָ�� client ����һ������
    ///
    bool SendCommand(
        unsigned int client_id,
        unsigned int command_id,
        void* data,
        size_t data_size,
        unsigned int time_out);

    ///
    // ��ָ�� client Ͷ��һ������
    ///
    bool PostCommand(
        unsigned int client_id,
        unsigned int command_id,
        void* data,
        size_t data_size);

private:
    // ί�д�����
    Delegate* delegate_;
};
