#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H


typedef void (*callback_function)(char a[]); // type for conciseness
//typedef void (*callback_messageRecieved)(int msgType, char data[]); // type for conciseness
typedef void (*callback_messageRecieved)(int msgType, char * data,int length); // type for conciseness

class clientserver
{
public:
    static const int MSG_ID = 0;
    static const int MSG_CONNECTION_ACCEPTED = 1; //to remote pc me enimerwse oti to aitima syndesis egine apodekto
    static const int MSG_REMOTE_CLIENT_ACCEPTED = 2; //apodextika to aitima syndesis pou ekane to remote pc.ksekinw na stelnw screenshots
    static const int MSG_SCREENSHOT = 3;

    clientserver();
    void start();
    void Connect(char RemoteID[]);
    void evt_id_recieved(const char *msg);
    void setDisconnectFunc(callback_function pFunc);
    void setMessageRecievedCallback(callback_messageRecieved func);
    void SendScreenshot(char message[]);
    void sendmsg(int socketfd,char command[],char message[]);
    void sendmsg2(int socketfd,char command[],char * message,int len);
    int localsocket;

private:
    void error(const char *msg);
    void proccesCommand(int socketfd,char * command);
    callback_function disconnectFunc;
    callback_messageRecieved messageRecievedCallback;
};

#endif // CLIENTSERVER_H
