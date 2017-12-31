// gia non blocking sockets diavasa apo edw:
// https://www.ibm.com/support/knowledgecenter/en/ssw_i5_54/rzab6/xnonblock.htm
// (Example: Nonblocking I/O and select())

#include "p2pserver.h"
#include "../Shared/AndamaHeaders/byte_functions.h"
#include "../Shared/AndamaHeaders/shared_constants.h"
#include "../Shared/AndamaHeaders/socket_functions.h"
#include "../Shared/General/finally.h"

#ifndef WIN32
#include <sys/ioctl.h> //gia non blocking sockets
#endif


#include <csignal> // xreiazetai gia na ginei compile se linux gia tin signal

P2PServer::P2PServer()
{
    isP2PCientConnected = false;
}

void P2PServer::setRemoteComputerOS(OS os)
{
    _remoteComputerOS = os;
}

void P2PServer::setConnectionState(connectionState state)
{
    std::lock_guard<std::mutex> lock(connection_state_mutex);
    m_connection_state = state;
}

void P2PServer::stopThread()
{
    _stopThread = true;
#ifdef WIN32
    shutdown(listensocket, SD_BOTH);
    closesocket(listensocket);
#else
    shutdown(listensocket, SHUT_RDWR);
    close(listensocket);
#endif
}

void P2PServer::run(void)
{
    this->start_p2pserver();
}

void P2PServer::sendDisconnectFromRemoteComputer(int socket)
{
    //1 byte
     setConnectionState(connectionState::connectedWithProxy);
    _sendmsgPlain(socket, CMD_DISCONNECT_FROM_REMOTE_COMPUTER);

#ifdef WIN32
     closesocket(socket);
#else
     //shutdown(socket,2);
     close(socket);
#endif

}


void P2PServer::start_p2pserver()
{
    int newsockfd;

    //std::set_terminate(onTerminate);

    //gia na mi prokaleitai crash otan paw na grapsw se socket pou exei kleisei
    //http://stackoverflow.com/questions/108183/how-to-prevent-sigpipes-or-handle-them-properly
    #ifdef WIN32
        //SOCKET socketfd = INVALID_SOCKET;
    #else
        //int socketfd;
    signal(SIGPIPE, SIG_IGN);
    #endif

    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;


#ifdef WIN32
    // Initialize Winsock
    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return;
    }
#endif

    listensocket = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
        if (listensocket == INVALID_SOCKET) {
#else
        if (listensocket < 0){
#endif
        // >>>>>>>>>>> error("ERROR opening socket");
            std::cout << "ERROR opening socket";
            return;
        }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT_NUMBER);

    //Xreiazetai wste se periptwsi crash na ginetai reuse to socket pou einai se state CLOSE_WAIT
    //mporw na to vrw se macos me: netstat -anp tcp | grep port_number
    int reuse = 1;
    if (setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

#ifdef SO_REUSEPORT
    if (setsockopt(listensocket, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEPORT) failed");
#endif

    if (bind(listensocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        // >>>>>>>>>>> error("ERROR on binding");
        std::cout << "p2pserver ERROR on binding" << std::endl;
        return;
    }

    listen(listensocket, SOMAXCONN);
    std::cout << "Listenning for connections on port: " << PORT_NUMBER << std::endl;

    clilen = sizeof(cli_addr);

    while (true && !_stopThread) {
                try {
                    newsockfd = accept(listensocket, (struct sockaddr *) &cli_addr, &clilen);
                    //int pHandle=new int;
                    if (listensocket < 0) {
                        // >>>>>>>>>>> error("ERROR on accept");
                    }

                    if (_stopThread) break;

                    std::cout << " New client accepted. Kalw accept_client_messages while loop se neo thread." << std::endl;

                    //thetw to recv timeout
        #ifdef WIN32
                    int iTimeout = 90000;
                    setsockopt(newsockfd,
                                       SOL_SOCKET,
                                       SO_RCVTIMEO,
                                       (const char *)&iTimeout,
                                       sizeof(iTimeout) );
        #else
                    struct timeval tv;
                    tv.tv_sec = 90;  /* 90 Secs Timeout */
                    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
                    setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
        #endif
                    //SIMANTIKO: kanw disable to nagle algorithm. meiwnei to latency.
                    int flag = 1;
                    setsockopt(newsockfd,                    /* socket affected */
                                            IPPROTO_TCP,     /* set option at TCP level */
                                            TCP_NODELAY,     /* name of option */
                                            (char *) &flag,  /* the cast is historical cruft */
                                            sizeof(int));    /* length of option value */

                    //diaxeirizomai ton neo client se neo thread.
                    auto t = std::thread(&P2PServer::future_thread_accept_client_messages, this, newsockfd, cli_addr.sin_addr.s_addr);
                    t.detach();
                } catch ( std::exception& ex) {
                    std::cout << "Accept loop exception: " << ex.what() << std::endl;
                }
                catch ( ... ) {
                    std::cout << "Unknown error in main loop" << std::endl;
                }
    } /* end of while */
    std::cout << "p2pserver select socket loop exiting...\r\n" << std::endl;
}

static std::atomic<int> activeP2PClientsCounter {0}; //TODO: na tin kanw member variable tis clasis. dokimasa alla evgaze compile errors
bool P2PServer::hasConnectedClientThreadsRunning()
{
    return (activeP2PClientsCounter > 0);
}

#ifdef WIN32
void P2PServer::future_thread_accept_client_messages(const SOCKET socketfd, const unsigned long clientIP){
#else
void P2PServer::future_thread_accept_client_messages(const int socketfd, const in_addr_t clientIP){
#endif
    activeP2PClientsCounter++;
    Finally fin([]{activeP2PClientsCounter--;});

    std::future<void> future_getNewP2PClient = std::async(std::launch::async, &P2PServer::accept_client_messages, this, socketfd, clientIP);
    future_getNewP2PClient.get();
    std::cout << "thread done!";
}

#ifdef WIN32
void P2PServer::accept_client_messages(const SOCKET socketfd, const unsigned long clientIP) {
#else
void P2PServer::accept_client_messages(const int socketfd, const in_addr_t clientIP) {
#endif
    //std::cout << getTime() << " " << std::this_thread::get_id() <<
    //             " Eisodos sto dostuff loop. Socket:%i" << socketfd  << std::endl;

    std::string myID = ""; // to trexon id tou client pou epikoinwnei me ton server se afto to thread
    std::vector<char>myIDv; // to idio me prin alla ws vector

    //efoson:
    //- to protocolo tou client einai apodekto
    //- o client exei lavei ID
    //- i IP tou client den einei banned - TODO
    //to handshakeCompleted ginetai true.
    //xrisimopoieitai ws ena epipleon epipedo asfaleias
    //wste na min mporei o client na steilei dedomena ston server
    //xwris kapoiou eidous eksousiodotisi
    bool handshakeCompleted = false;


    isP2PCientConnected = false;
    activeClientSocket = socketfd;

    std::array<char,1> cmdbuffer; // o buffer pou lamvanei to trexon command kathe fora
    int cmd_bytes_recv; // ta bytes pou lifthikan gia to command. Prepei na einai 1.

    try
    {
        //molis syndethei o client, o server apostelei to
        //megisto protocol version poy ypostirizei.
        std::vector<char> server_protocol(2);
        server_protocol[0] = '1';
        server_protocol[1] = '0';
        _sendmsgPlain(socketfd, CMD_PROTOCOL, server_protocol);
    }
    catch (std::exception ex)
    {
        std::cout << "----------> EXCEPTION ********  Synevh exception stin arxi [CMD_PROTOCOL], sto dostuff while loop: " << ex.what() << std::endl;
        // >>>>>>>>>>>>>> displayErrno("----------> EXCEPTION [CMD_PROTOCOL] dostuff loop");
        return;
    }


    //ksekinw tin epesksergasia twn commands pou
    //endexetai na stelnei o client
    while (true) {

        try {
            //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
            //             " 1. Anamoni command..." << std::endl;


            // Kyrio simeio. Lamvanw to command (1 byte)
            cmd_bytes_recv = recv(socketfd, &cmdbuffer[0], 1, 0);
            //std::cout << "DOSTUFF BYTES Received: " << cmd_bytes_recv << std::endl;

            if (cmd_bytes_recv == 0){
                // >>>>>>>>>>>>>>  std::cout << getTime() << " " << std::this_thread::get_id() << " " <<
                //             "######### --- Main command loop disconnected from client. ---- ########" << " " <<
                //           "####  recv return 0 bytes. [MAIN command loop]. Returning from function." << std::endl;
                // >>>>>>>>>>>>>> cleanup(myID, socketfd);
                return;
            }
            else if (cmd_bytes_recv == -1){
                //edw erxetai kai otan simvei timeout
                std::cout << "----------> Client heartbeat timeout" << std::endl;
            // >>>>>>>>>>>>>>     displayErrno("void dostuff(int newsockfd) - [MAIN command loop] - [cmd_bytes_recv == -1]");
            // >>>>>>>>>>>>>>     cleanup(myID, socketfd);
                return;
            }

            //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
            //             " 2. Ellava bytes: " << cmd_bytes_recv << ", command: " << &cmdbuffer[0] <<  " Ksekinw epeksergasia tou command" << std::endl;


            if (cmdbuffer == CMD_PROTOCOL) {
                //o client stelnei to app version kai to protocol version pou ypostirizei
                //sti morfi "P<app_major><app_minor><app_revision><protocol_major><protocol_minor><protocol_revision>" opou major,minor,revision apo ena byte.
                //P<app_major><app_minor><app_revision><protocol_major><protocol_minor><protocol_revision><cachedID length 1 bytes> <cachedID>
                //synolo P command 6 bytes <<<<<

                //-- std::vector<char> client_protocolbuff(6);
                //-- _receivePlain(socketfd, client_protocolbuff); //<-----------------------------

                std::cout << "Will recieve protocol info" << std::endl;
                std::vector<char> client_protocolbuff;
                _receive(socketfd, client_protocolbuff); //<-----------------------------
                std::cout << "examining client protocol" << std::endl;
                std::string vapp_maj(client_protocolbuff.begin(),    client_protocolbuff.begin()+1);
                std::string vapp_min(client_protocolbuff.begin() +1, client_protocolbuff.begin()+2);
                std::string vapp_rev(client_protocolbuff.begin() +2, client_protocolbuff.begin()+3);

                std::string vprot_maj(client_protocolbuff.begin()+3, client_protocolbuff.begin()+4);
                std::string vprot_min(client_protocolbuff.begin()+4, client_protocolbuff.begin()+5);
                std::string vprot_rev(client_protocolbuff.begin()+5, client_protocolbuff.begin()+6);
                std::cout << "examining client protocol. app version and protocol version ok" << std::endl;
                //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                //             " Client protocol: v" << v_maj << "." << v_min << std::endl;

                //edw elegxw ean to client app version kai protocol version einai apodekta
                //prin dimiourgisw new id gia ton client
                //elegxw prota to app version
                if (vapp_maj != "0" || vapp_min != "1"){
                    //mi apodekto version efarmogis.
                    //enimerwnw ton xristi na kanei update
                    std::string app_update_url = "http://forum.andama.org";
                    std::vector<char> vapp_update_url(app_update_url.begin(), app_update_url.end());
                    _sendmsg(socketfd, CMD_ERROR_APP_VERSION_NOT_ACCEPTED, vapp_update_url);
                    return;
                }

                //to p2p protocol einai ok opote enimerwnw ton client
                _sendmsgPlain(socketfd, CMD_P2P_PROTOCOL_OK);

                // >>>>>>>>>>>>>>
                /*
                std::vector<char> cachedIDSize(client_protocolbuff.begin() + 6,client_protocolbuff.begin() + 7);
                int icachedIDSize = bytesToInt(cachedIDSize);
                //efou gnwrizoume to length tou id, lamvanoume to id sto vector
                std::vector<char> cachedID(client_protocolbuff.begin() + 7,client_protocolbuff.begin() + 7 + icachedIDSize);

                //dimiourgw id kai ton kataxwrw sto dictionary map
                // >>>>>>>>>>>>>> myID = getClientID(socketfd, cachedID);
                myIDv.insert(myIDv.end(),myID.begin(),myID.end());//to id

                std::vector<char> buffsendID; //to synoliko minima pou tha stalei

                //kataxwrw to megethos tou ID kai to ID
                std::vector<char> buffLenID(1);
                intToBytes(myID.length(),buffLenID);
                buffsendID.insert(buffsendID.end(), buffLenID.begin(),buffLenID.end());
                buffsendID.insert(buffsendID.end(), myID.begin(),myID.end());

                //kataxwrw to megethos to cachedID kai to cachedID
                std::vector<char> buffLenCachedID(1);
                intToBytes(clients[myID].cachedID.size(),buffLenCachedID);
                buffsendID.insert(buffsendID.end(), buffLenCachedID.begin(),buffLenCachedID.end());
                buffsendID.insert(buffsendID.end(), clients[myID].cachedID.begin(),clients[myID].cachedID.end());


                //Stelnw to ID kai to cachedID ston client
                _sendmsg(socketfd, CMD_ID, buffsendID); //>-------------------------
                */

                //prosthetw ton client sto dictionary me olous tous clients
               // { // lock_guard scope
               //     std::lock_guard<std::mutex> lock (clients_mutex);
               //     ClientInfo ci;
               //     ci.remote_socket = 0;
               //     ci.sockfd = socketfd;
               //     clients[myID] = ci;
               // } // lock_guard scope

                handshakeCompleted = true;
                isP2PCientConnected = true;

                // >>>>>>>>>>>>>> std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                //             " CMD_PROTOCOL >  Client ID created and sent to client." << std::endl;

            } // CMD_PROTOCOL

            //proothisi arxikou screenshot ston paralipti
            else if (cmdbuffer == CMD_SCREENSHOT && handshakeCompleted) {
                std::vector<char> screenshot_data_buff;
                _receive(socketfd, screenshot_data_buff);

                // >>>>>>>>>>>>>> _sendmsg(getRemoteComputerSocket(myID), CMD_SCREENSHOT,
                //        screenshot_data_buff);//>--------------
            } // CMD_SCREENSHOT

            //proothisi screenshot diff ston paralipti
            else if (cmdbuffer == CMD_SCREENSHOT_DIFF && handshakeCompleted) {
                //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                //                 " 6.0. CMD_SCREENSHOT_DIFF > Tha ginei recieve twn dedomenwn eikonastou screenshot diff." << std::endl;
                std::vector<char> screenshot_diff_data_buff;
                _receive(socketfd, screenshot_diff_data_buff);

                //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                //             " 6.1. CMD_SCREENSHOT_DIFF > lifthike. Bytes: " << bytes_received << std::endl;

                // >>>>>>>>>>>>>> _sendmsg(getRemoteComputerSocket(myID), CMD_SCREENSHOT_DIFF,
                //        screenshot_diff_data_buff);//>--------------

                //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                //             " 7. CMD_SCREENSHOT_DIFF > To screenshot diff proothithike pros: " << getRemoteComputerID(myID) << " Bytes send: " << bytes_sent << std::endl;
            } // CMD_SCREENSHOT_DIFF

            else if(cmdbuffer == CMD_P2P_CONNECT)
            {
                std::cout << "entering CMD_P2P_CONNECT " << std::endl;

                //TO CMD_P2P_CONNECT exei tin morfi:
                //[remote computer os 1 byte][password ta ypoloipa bytes]
                std::vector<char> remote_client_idbuff;
                _receive(socketfd, remote_client_idbuff); //lamvanw olokliro to message

                //to remote computer os
                std::vector<char> connRemoteComputerOS(remote_client_idbuff.begin(), remote_client_idbuff.begin() + 1);
                setRemoteComputerOS((OS)bytesToInt(connRemoteComputerOS));

                //sti nynexeia lamvanoume to password size
                std::vector<char> pwdSize(remote_client_idbuff.begin() + 1, remote_client_idbuff.begin() + 2);
                int ipwdSize = bytesToInt(pwdSize);

                //lamvanw to password
                std::vector<char> vpassword(remote_client_idbuff.begin() + 2, remote_client_idbuff.begin() + 2 + ipwdSize);

                //elegxw ean exei ginei ban i IP
                if (clientserver::isIPBannedForWrongPasswords(clientIP,socketfd) == true){
                    sendDisconnectFromRemoteComputer(socketfd);
                    return;
                }

                //elegxw ean to password pou stalthike einai to idio me to password pou exei o client edw
                std::string passwdQuestion(vpassword.begin(), vpassword.end());
                if (passwdQuestion == this->password){
                        //to password pou stalthike einai swsto
                        std::cout << "Client connection accepted. Remote Client ID: "<< remote_client_idbuff.data() << std::endl;

                        //kanw reset to password protection gia thn sygekrimenh ip
                        clientserver::resetWrongPasswordIPProtection(clientIP);

                        setConnectionState(connectedWithOtherAsServer);

                        _sendmsg(socketfd, CMD_ACCEPT, std::vector<char>());//>-------------stelnw sto server oti egine accept tou Andama request, stelnontas to remote id
                        //std::vector<char> emtpyv;
                        //emit sig_messageReceived(this, MSG_REMOTE_CLIENT_ACCEPTED, remote_client_idbuff);
                        emit sig_messageReceived(NULL, 2, remote_client_idbuff);
                    } else {
                        //to password pou stalthike einai lathos
                        //lamvanw apo to vector tin ip tou client
                        clientserver::addWrongPasswordIPProtection(clientIP, socketfd);
                        sendDisconnectFromRemoteComputer(socketfd);
                        std::cout << "will exit thread: addWrongPasswordIPProtection " << std::endl;
                        return;
                    }
            } // CMD_P2P_CONNECT


            //proothisi aitimatos gia syndesi se apomakrismeno ypologisti
            else if (cmdbuffer == CMD_CONNECT && handshakeCompleted) {
                std::vector<char> remote_client_idbuff;
                _receive(socketfd, remote_client_idbuff); //<-----------to remote client id pou zitithike kai to remote password

                // >>>>>>>>>>>>>>
                /*
                //ean exei ginei idi ban stin IP den kanw tipota allo
                if(isBannedForWrongIDs(socketfd,clientIP)){
                    continue;
                }
                */

                //agnoow to 1o byte kathws einai to msgtype


                //to 2o byte einai to os
                //std::vector<char> connRemoteComputerOS(remote_client_idbuff.begin() + 1, remote_client_idbuff.begin() + 2);
                //int os = bytesToInt(connRemoteComputerOS);

                //lamvanw to 3o byte poy mas deixnei to size tou ID
                std::vector<char> idSize(remote_client_idbuff.begin() + 2, remote_client_idbuff.begin() + 3);
                int iIDSize = bytesToInt(idSize);
                //efou gnwrizoume to length tou id, lamvanoume to id sto vector
                std::vector<char> vremoteID(remote_client_idbuff.begin() + 3, remote_client_idbuff.begin() + 3 + iIDSize);

                //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                //             " CMD_CONNECT > Zitithike syndesi pros client me ID: " << &remote_client_idbuff[0] << std::endl;

                // >>>>>>>>>>>>>> std::lock_guard<std::mutex> lock (clients_mutex);

                //anazitisi ean to remote client id einai kataxwrimeno sto dictionary
                // >>>>>>>>>>>>>>
                //std::string sid(vremoteID.begin(),vremoteID.end());
                //std::map<std::string, ClientInfo>::iterator found_client =
                //        clients.find(sid);
                //if (found_client != clients.end()) {
                    //to remote id pou zitithike vrethike.

                    //lamvanw to size tou password
                    std::vector<char> pwdSize(remote_client_idbuff.begin() + 3 + iIDSize, remote_client_idbuff.begin() + 3 + iIDSize + 1);
                    int ipwdSize = bytesToInt(pwdSize);

                    //lamvanw to password apo to vector poy ellava prin
                    std::vector<char> vpassword(remote_client_idbuff.begin() + 3 + iIDSize + 1, remote_client_idbuff.begin() + 3 + iIDSize + 1 + ipwdSize);

                    std::vector<char> connect_data;
                    // >>>>>>>>>>>>>> createConnectCommandData(connect_data,myIDv,vpassword,clientIP,os);


                    //stelnw sto remote clientid pou zitithike request gia syndesi
                    //kai perimenw tin apantisi
                    // >>>>>>>>>>>>>> clients[sid].remote_socket = socketfd;//theto sto remote client to socket gia epikoinwnia me to client pou zitise tin epikoinwnia
                    // >>>>>>>>>>>>>> _sendmsg(clients[sid].sockfd, CMD_CONNECT, connect_data);//>---------stelnw sto remote client connect request me to client id pou to zitise
                    //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                    //             " CMD_CONNECT > To remote client id: [" << &remote_client_idbuff[0] << "] verithike kai stalthike CMD_CONNECT." << std::endl;
                //} else {
                    //to remote id pou zitithike den vrethike
                    //opote enimerwnw to id protection
                    //gia to gegonos oti o client anazitise id pou den yparxei (id sniffing protection)
                 //   add_wrong_ID(socketfd,clientIP);
                //}

            } // CMD_CONNECT

            else if (cmdbuffer == CMD_CONNECT_PASSWORD_NOT_CORRECT && handshakeCompleted) {
                // >>>>>>>>>>>>>> _sendmsgPlain(getRemoteComputerSocket(myID), CMD_CONNECT_PASSWORD_NOT_CORRECT);//>--------------
            } // CMD_CONNECT_PASSWORD_NOT_CORRECT

            else if (cmdbuffer == CMD_BAN_IP_WRONG_PWD && handshakeCompleted) {
                // >>>>>>>>>>>>>> _sendmsgPlain(getRemoteComputerSocket(myID), CMD_BAN_IP_WRONG_PWD);//>--------------
            } // CMD_BAN_IP_WRONG_PWD

            else if (cmdbuffer == CMD_WARNING_BAN_IP_WRONG_PWD && handshakeCompleted) {
                std::vector<char> remain_tries_buff(1);
                _receivePlain(socketfd, remain_tries_buff);
                // >>>>>>>>>>>>>> _sendmsgPlain(getRemoteComputerSocket(myID), CMD_WARNING_BAN_IP_WRONG_PWD, remain_tries_buff);//>--------------
            } // CMD_WARNING_BAN_IP_WRONG_PWD


            //proothisi apodoxis syndesis
            else if (cmdbuffer == CMD_ACCEPT && handshakeCompleted){
                //o client apantise thetika sto aitima gia syndesi
                //stelnontas to remote id pou prokalese to connect
                std::vector<char> accepted_client_id_buff;
                _receive(socketfd, accepted_client_id_buff); // <--------to remote id pou egine accept

                // >>>>>>>>>>>>>> std::lock_guard<std::mutex> lock (clients_mutex);

                // >>>>>>>>>>>>>>
                /*
                //anazitisi ean to remote id pou egine accept yparxei sto dictionary
                std::string sid(accepted_client_id_buff.begin(),accepted_client_id_buff.end());
                std::map<std::string, ClientInfo>::iterator found_client =
                        clients.find(sid);
                if (found_client != clients.end()) {
                    { // lock_guard scope
                        clients[sid].remote_socket = socketfd;
                        clients[sid].remote_id = myID;
                        clients[myID].remote_id = sid;
                    } // lock_guard scope

                    //std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                    //             " CMD_ACCEPT > Egine apodoxi apo ton client. Enimerwnw ton kalounta me id: " << &accepted_client_id_buff[0] << std::endl;

                    _sendmsg(clients[sid].sockfd, CMD_ACCEPT, myIDv); //TODO: <edw na stelnw to id aftounou pou ekane accept
                }
                else {
                     std::cout << getTime() << " " << std::this_thread::get_id() << " " << myID <<
                                 " CMD_ACCEPT > Stalthike accept apo ton client alla o paraliptis client den vrethike: " << &accepted_client_id_buff[0] << std::endl;
                }
                */
            } // CMD_ACCEPT

            //proothisi aitimatos aposyndesis
            else if (cmdbuffer == CMD_DISCONNECT_FROM_REMOTE_COMPUTER && handshakeCompleted) {
                // >>>>>>>>>>>>>> _sendmsgPlain(getRemoteComputerSocket(myID), CMD_DISCONNECT_FROM_REMOTE_COMPUTER);//>--------------

                // >>>>>>>>>>>>>>
                /*
                std::lock_guard<std::mutex> lock (clients_mutex);

                clients[myID].remote_socket = -1;

                clients[clients[myID].remote_id].remote_socket = -1;
                clients[clients[myID].remote_id].remote_id= "";

                clients[myID].remote_id = "";
                */

            } // CMD_DISCONNECT_FROM_REMOTE_COMPUTER

            else if(cmdbuffer == CMD_REQUEST_SCREENSHOT)
            {
                //qDebug("Lipsi aitimatos apostolis screenshot.");

                std::vector<char> emptyv;
                //std::vector<char> cdata;
                emit sig_messageReceived(NULL, 5, emptyv);
            } // CMD_REQUEST_SREENSHOT

            else if(cmdbuffer == CMD_REQUEST_SCREENSHOT_DIFF)
            {
                //lamvanw to id tou request
                std::vector<char> screenshot_diff_id_data_buff;
                _receive(socketfd, screenshot_diff_id_data_buff);
                std::string rid(screenshot_diff_id_data_buff.begin(), screenshot_diff_id_data_buff.end());

                //qDebug ("4. CMD_REQUEST_SCREENSHOT_DIFF ID: %s. Lipsi aitimatos apostolis screenshot diff. Will emit signal from protocol to ui. Bytes recv: %i",rid.c_str(),bytes_received);
                //std::vector<char> cdata;
                emit sig_messageReceived(NULL, 6, screenshot_diff_id_data_buff);
            } // CMD_REQUEST_SCREENSHOT_DIFF

            //proothisi mouse
            else if(cmdbuffer == CMD_MOUSE && handshakeCompleted)
            {
                std::vector<char> mouse_data_buff(10);
                _receivePlain(socketfd, mouse_data_buff);

                //std::vector<char> cdata;
                emit sig_messageReceived(NULL, 7, mouse_data_buff);
            }

            //proothisi keyboard
            else if(cmdbuffer == CMD_KEYBOARD && handshakeCompleted)
            {
                std::vector<char> keyboard_data_buff;
                _receive(socketfd, keyboard_data_buff);

                //std::vector<char> cdata;
                //emit sig_messageReceived(NULL, MSG_KEYBOARD,keyboard_data_buff);
                emit sig_messageReceived(NULL, 8, keyboard_data_buff);
            } // CMD_KEYBOARD

            //hearbeat. Ean den lifthei sto xrono tou socket timeout
            //tote diakoptetai i syndesi me ton server
            else if (cmdbuffer == CMD_HEART_BEAT && handshakeCompleted) {
                //den xreiazetai na kanw kati. apla diatirei ton client syndedemeno
                //kathws den energopoieitai to timeout sto recv tou command
                //std::cout << "------- >>> HeartBeat\n";
            }


            else {
                std::cout << "----> AGNWSTO COMMAND!! :" << &cmdbuffer[0] << std::endl;
                //den einai kapoio gnwsto command opote termatizw ti syndesi me to xristi
                // >>>>>>>>>>>>>> cleanup(myID, socketfd);
                return;
            }

        } catch (std::exception& ex) {
            //ean simvei kapoio exception p.x. den mporese na ginei proothisi dedomenwn
            //sto remote socket appla katapinw to sfalma.
            std::cout << "----------> EXCEPTION ********  Synevh exception sto dostuff while loop: " << ex.what() << std::endl;
            // >>>>>>>>>>>>>> displayErrno("----------> EXCEPTION dostuff loop");

            // >>>>>>>>>>>>>> cleanup(myID, socketfd);

            return;
        }
        catch ( ... ) {
            std::cout << "Unknown error in dostuff loop" << std::endl;
            return;
        }
    } // while proccessCommand
}




