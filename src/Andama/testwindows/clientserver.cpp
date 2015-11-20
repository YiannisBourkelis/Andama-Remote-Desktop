#include <ws2tcpip.h>
#include <stdio.h>
#include "winsock2.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#define close(s) closesocket(s)
//#define write(a,b,c) send(a,b,c,NULL)
#pragma comment(lib, "Ws2_32.lib")

#include "clientserver.h"
#include "qdebug.h"

char CMD_PROTOCOL[] = "P";
char CMD_CHAT[] = "c";
char CMD_CONNECT[] = "C";
char CMD_QUIT[] = "Q";
char CMD_ACCEPT[] = "A";
char CMD_ID[] = "I";
char CMD_SCREENSHOT[] = "S";

clientserver::clientserver()
{
    disconnectFunc=0;
    messageRecievedCallback=0;
}

const struct hostent *SERVER = gethostbyname("mailgate.filoxeni.com");
//const struct hostent *SERVER = gethostbyname("localhost");
const int PORT_NUMBER = 8097;

void clientserver::setMessageRecievedCallback(callback_messageRecieved func)
{
   messageRecievedCallback = func;
}

void clientserver::setDisconnectFunc(callback_function pFunc)
{
    disconnectFunc = pFunc; // store
}

void clientserver::error(const char *msg)
{
perror(msg);
exit(0);
}

void clientserver::SendScreenshot(char message[])
{
    int len = sizeof(message)+1; //strlen(message);
    qDebug("sendScreenshot sizeof: %i",len);
    sendmsg(localsocket,CMD_SCREENSHOT,message);
}

void intToBytes(int value, char *buffer, int bytes)
{
if(bytes == 4){
    buffer[0] = (value >> 24) & 0xff;
    buffer[1] = (value >> 16) & 0xff;
    buffer[2] = (value >> 8)  & 0xff;
    buffer[3] = value & 0xff;
}
else if(bytes == 1){
    buffer[0] = value & 0xff;
}
}

int bytesToInt(char *buffer,int bytes)
{
if (bytes==4){
    return
        (((unsigned char)buffer[0]) << 24) +
        (((unsigned char)buffer[1]) << 16) +
        (((unsigned char)buffer[2]) << 8) +
          (unsigned char)buffer[3];
}
else if (bytes == 1){
    return
          (unsigned char)buffer[0];
}
return 0;
}


void clientserver::sendmsg2(int socketfd,char command[],char * message,int len)
{
send(socketfd,command,1,NULL);

char lenb[4];
intToBytes(len,lenb,4);
send(socketfd,lenb,4,NULL);
qDebug("new len: %i",len);

send(socketfd,message,len,NULL);
}

void clientserver::sendmsg(int socketfd,char command[],char message[])
{
int len = sizeof(message)+1; //strlen(message);

qDebug("message len: %i",len);

send(socketfd,command,1,NULL);

char lenb[4];
intToBytes(len,lenb,4);
send(socketfd,lenb,4,NULL);

send(socketfd,message,len,NULL);
}


/*
getData(int sockfd,char* outChar){
char lenb[4];
bzero(lenb,4);
recv(sockfd,lenb,4,MSG_WAITALL);
int len = bytesToInt(lenb,4);
char buffer[len];
bzero(buffer,len);
recv(sockfd,buffer,len,MSG_WAITALL);
return buffer;
}
*/

char myID[20];
bool autoChat = true;
long chatMessageCounter = 0;

void clientserver::Connect(char RemoteID[])
{
    sendmsg(localsocket,CMD_CONNECT,RemoteID);//>---------------------aitisi connect se allo ypologisti
}

void clientserver::proccesCommand(int socketfd,char * command){
char cmd[2];
bzero(cmd,2);
cmd[0]=command[0];

    if(strcmp(cmd,CMD_PROTOCOL)==0)
    {
        //o server stelnei tin version toy protocol pou ypostirizei
        //sti morfi "P<major><minor>" opou major,minor apo ena byte.
        //synolo command 3 bytes
        char pmajor[1],pminor[1];
        recv(socketfd,pmajor,1,0);
        recv(socketfd,pminor,1,0);
        int pimajor=bytesToInt(pmajor,1);
        int piminor=bytesToInt(pminor,1);
        printf("Server protocol: v%d.%d\n",pimajor,piminor);

        qDebug() << "sdsddsdsddssdds";

        //otan o server mou stelnei to protocol pou ypostirizei
        //tote tou stelnw kai egw afto pou ypostirizei o client
        //stin idia morfi me prin
        int pimajor_s=1;
        int piminor_s=0;
        char pmajor_s[1],pminor_s[1];
        intToBytes(pimajor_s,pmajor_s,1);
        intToBytes(piminor_s,pminor_s,1);
        send(socketfd,CMD_PROTOCOL,1,0);
        send(socketfd,pmajor_s,1,0);
        send(socketfd,pminor_s,1,0);
    }

    else if(strcmp(cmd,CMD_ID)==0)
    {
        //efoson o server apodextei to protocol verion
        //mou stelnei to id tou client sti morifi: I<plength><id>

        char lenb[4];
        bzero(lenb,4);
        recv(socketfd,lenb,4,MSG_WAITALL);//<-------------------------length tou id
        int len = bytesToInt(lenb,4);
        char myID[20];
        bzero(myID,20);
        recv(socketfd,myID,len,MSG_WAITALL);//<---------------------to client id pou mou esteile o server
        printf("Client ID recieved from server: %s\n",myID);

        if(disconnectFunc!=0){
            disconnectFunc(myID);
        }

        if(messageRecievedCallback != 0){
            messageRecievedCallback(MSG_ID, myID,-1);
        }


        //afou exw lavei client id zitw an thelei o xristis na kanei connect i listen gia chat
        /*
        printf("Pliktrologiste 1 gia syndesi, 2 gia aftomati syndesi, 3 gia anamoni, 4 gia aftomati anamoni:");
        char synbuff[256];
        bzero(synbuff,256);
        fgets(synbuff,255,stdin);

        char syndesi[256] = "1";
        char syndesi_auto[256] = "2";
        char anamoni_auto[256] = "4";

        if(synbuff[0]==syndesi[0] || synbuff[0]==syndesi_auto[0]){
            printf("pliktrologiste to id tou apomakrismenou ypologisti:");
            bzero(synbuff,256);
            fgets(synbuff,255,stdin);
            strtok(synbuff,"\n");
            sendmsg(socketfd,CMD_CONNECT,synbuff);//>---------------------aitisi connect se allo ypologisti
            printf("anamoni apodoxis syndesis...\n");
            if(synbuff[0]==syndesi_auto[0])
            {
                autoChat = true;
            }
        }
        else if(synbuff[0] == anamoni_auto[0])
        {
            printf("aftomati anamoni...");
            autoChat=true;
        }
        else
        {
            printf("anamoni...");
        }
        */
    } // CMD_ID


    else if(strcmp(cmd,CMD_CONNECT)==0)
    {
        char lenb2[4];
        bzero(lenb2,4);
        recv(socketfd,lenb2,4,MSG_WAITALL);//<-----------length tou remote client id pou prokalese to connect
        int len2 = bytesToInt(lenb2,4);
        char conn[20];
        bzero(conn,20);
        recv(socketfd,conn,len2,MSG_WAITALL);//<-----------to remote client id pou prokalese to connect

        printf("Client connection accepted. Remote Client ID: %s\n",conn);
        sendmsg(socketfd, CMD_ACCEPT, conn);//>-------------stelnw sto server oti egine accept tou remote connect request, stelnontas to remote id

        if(messageRecievedCallback != 0){
            messageRecievedCallback(MSG_REMOTE_CLIENT_ACCEPTED, myID,-1);
        }
    } // CMD_CONNECT

    else if(strcmp(cmd,CMD_ACCEPT)==0)
    {
        if(messageRecievedCallback != 0){
            messageRecievedCallback(MSG_CONNECTION_ACCEPTED, NULL,-1);
        }
        /*
        printf("O apomakrismenos ypologistis apodextike tin syndesi!\n Grapse kati \n");
        printf("Local > ");
        char chat_buff[256];
        bzero(chat_buff,256);
        fgets(chat_buff,255,stdin);
        strtok(chat_buff,"\n");
        sendmsg(socketfd,CMD_CHAT,chat_buff);
        */
    }

    else if(strcmp(cmd,CMD_CHAT)==0)
    {
        char len_chat_bytes[4];
        bzero(len_chat_bytes,4);
        recv(socketfd,len_chat_bytes,4,MSG_WAITALL);
        int len_chat_data = bytesToInt(len_chat_bytes,4);
        char chat_data_buff[255];
        bzero(chat_data_buff,255);
        recv(socketfd,chat_data_buff,len_chat_data,MSG_WAITALL);

        printf("Remote > %s\n",chat_data_buff);

        printf("[%lu] Local > ", chatMessageCounter++);
        //char chat_buff2[256];
        char chat_buff2[] = "ewuh wuhewffeui uih IUEHUIDH ewudhewdiuehw iu213h34234324 3uiwehf wefuhfkfh <keshfuwekfhs <ekfheskufh<kfheskufe f<esufgej<fg<f <dhsbf ejrfb <sejfbs<hjfb<ejyf serfjyesb fefub<es fjbesfjesbf efjbse jfsfb bej<efbesf ef";
        if (autoChat == true){
        }
        else
        {
            bzero(chat_buff2,500);
            fgets(chat_buff2,255,stdin);
            strtok(chat_buff2,"\n");
        }
        sendmsg(socketfd,CMD_CHAT,chat_buff2);
    }//CMD_CHAT

    else if(strcmp(cmd,CMD_SCREENSHOT)==0)
    {
        char len_screenshot_bytes[4];
        bzero(len_screenshot_bytes,4);
        recv(socketfd,len_screenshot_bytes,4,MSG_WAITALL);
        int len_screenshot_data = bytesToInt(len_screenshot_bytes,4);
        char * screenshot_data_buff = new char[len_screenshot_data]; //char screenshot_data_buff[len_screenshot_data];
        bzero(screenshot_data_buff,len_screenshot_data);
        recv(socketfd,screenshot_data_buff,len_screenshot_data,MSG_WAITALL);

        qDebug("screenshot recieved! total bytes: %i",len_screenshot_data);

        //for(int i=0;i<len_screenshot_data;i++){
        //    qDebug("socket start: %i, byte: %i",i,screenshot_data_buff[i]);
        //}

        if(messageRecievedCallback != 0){
            messageRecievedCallback(MSG_SCREENSHOT, screenshot_data_buff,len_screenshot_data);
        }
    }
}


//static thread_local unsigned int rage = 1;

void clientserver::start()
{
int sockfd, n;
struct sockaddr_in serv_addr;

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
    error("ERROR opening socket");

if (SERVER == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
}

bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)SERVER->h_addr,
     (char *)&serv_addr.sin_addr.s_addr,
     SERVER->h_length);
serv_addr.sin_port = htons(PORT_NUMBER);
if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");

localsocket = sockfd;

printf("Remote Cotrol BETA - (c) 2014 Yiannis Bourkelis \n\n");

//ksekinw tin epesksergasia twn commands
char cmdbuffer[1]={};

while(true){
    n=recv(sockfd,cmdbuffer,1,0);
    if (n<=0){
        printf("Main command loop disconnected from server.\n");
        exit(0);
    }

    proccesCommand(sockfd,cmdbuffer);
}

close(sockfd);
return;
}
