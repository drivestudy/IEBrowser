#pragma once

///
// ����һ�Զ�Ľ��̼�ͨ��, (һ�� server ���̺Ͷ�� client ����)
// ��װ client �����ϵ�ͨ�Žӿ�
///
class IPCClient
{
public:
    class Delegate
    {
    public:
        ///
        // �յ����� server ������
        ///
        virtual void OnReceiveServerCommand(
            unsigned int command_id,
            void* data,
            size_t data_size) {};
    };

private:
    IPCClient();
    ~IPCClient();

    ///
    // ��ȡ����
    ///
    static IPCClient* GetInstance();

    ///
    // ���ӵ� server
    ///
    bool Connect();

    ///
    // �� server �Ͽ�����
    ///
    void DisConnect();

    ///
    // ����ί��
    ///
    void SetDelegate(Delegate* delegate);

    ///
    // �Ƴ�ί��
    ///
    void RemoveDelegate();

    ///
    // �� server ����һ������
    ///
    bool SendCommand(unsigned int command_id, void* data, size_t data_size, unsigned int time_out);

    ///
    // �� server Ͷ��һ������
    ///
    bool PostCommand(unsigned int command_id, void* data, size_t data_size);

private:
    // ί�д�����
    Delegate* delegate_;
};
