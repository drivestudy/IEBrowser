# IPC Message Define

明确一下通信时收发的格式。

消息存储在 IPCBuffer 类里面，它存储消息的格式是：
- message : 消息 id
- ex_message : 附加消息 id
- client_id : 客户端 id, 如果是 server 发往 client 的消息，则没有这个字段
- data : 消息的数据

通知消息的格式不太一样，它不通过 IPCBuffer 来存储(因为没有数据)，而是存储在 PostMessage 的 WPARAM, LPARAM 参数里面

PostMessage(window_handle, message, w_param, l_param);
- message : 消息 id
- w_param : client_id
- l_param : 附加消息 id