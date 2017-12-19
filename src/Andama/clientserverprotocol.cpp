
/* ***********************************************************************
 * Andama
 * (C) 2017 by Yiannis Bourkelis (hello@andama.org)
 *
 * This file is part of Andama.
 *
 * Andama is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Andama is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
 * ***********************************************************************/

#include "clientserverprotocol.h"
#include "../Shared/Cryptography/openssl_aes.h"


static std::mutex protect_password_mutex;
static std::map<in_addr_t,PasswordProtection> protect_password;//krataei tis ip pou exoun kanei apopeira syndesis me password pou einai lathos
static std::mutex sendmutex;
static std::string remotepassword;



clientServerProtocol::clientServerProtocol()
{
    setRemoteComputerOS(helperfuncs::OS::Unknown);
    //this->_protocolSupervisor = protocol_supervisor;
}



void clientServerProtocol::proccessMessage(const std::array<char, 1> &command)
{
    //qDebug("3. Inside proccessCommand. Checking received command: %s",command.data());

    if(command == CMD_PROTOCOL)
    {
        //std::string ww ("yian");
        //cachedID.insert(cachedID.begin(),ww.begin(),ww.end());

        //o server stelnei tin version toy protocol pou ypostirizei
        //sti morfi "P<major><minor>" opou major,minor apo ena byte.
        //synolo command 3 bytes
        std::vector<char> protocolbuff(2);
        _receivePlain(activeSocket, protocolbuff);
        qDebug("Server protocol: v%s.%s", &protocolbuff[0], &protocolbuff[1]);

        //otan o server mou stelnei to protocol pou ypostirizei
        //tote tou stelnw kai egw afto pou ypostirizei o client
        //stin idia morfi me prin
        std::vector<char> client_appver_and_protocolver(7);

        //TODO: na lamvanei to app version apo metavlites sto client.pro
        //app version
        client_appver_and_protocolver[0] = '0';
        client_appver_and_protocolver[1] = '1';
        client_appver_and_protocolver[2] = '0';

        //protocol version
        client_appver_and_protocolver[3] = '0';
        client_appver_and_protocolver[4] = '1';
        client_appver_and_protocolver[5] = '0';

        std::vector<char> cachedIDSize(1);
        intToBytes(cachedID.size(), cachedIDSize);
        client_appver_and_protocolver[6] = cachedIDSize[0];

        if (cachedID.size() > 0){
            //efoson yparxei apothikevmeno cachedID to stelnw ston server wste
            //na mou epistrepsei to idio ID
            client_appver_and_protocolver.insert(client_appver_and_protocolver.begin()+7, cachedID.begin(), cachedID.end());
        }

        //stelnw to version tis efarmogis, tou protocolou kai tyxwn cachedID me ti morfi:
        //<CMD_PROTOCOL> <client_appver_and_protocolver 6 bytes> <cachedID length 1 bytes> <cachedID>
        //_sendmsgPlain(this->getActiveSocket(),CMD_PROTOCOL,client_appver_and_protocolver);
        _sendmsg(activeSocket, CMD_PROTOCOL, client_appver_and_protocolver);
    }

    else if(command == CMD_P2P_PROTOCOL_OK)
    {
        //isClientConnected = true;
        //emit sig_messageReceived(this, MSG_CONNECTION_ACCEPTED, std::vector<char>());
        qDebug() << ">>>>>>>>>>>>>>  CMD_P2P_PROTOCOL_OK:" << QString(remotepassword.c_str());
        std::vector<char> pp = std::vector<char>(remotepassword.begin(), remotepassword.end());
        ConnectP2P(pp);
    }

    else if(command == CMD_ID)
    {
        //efoson o server apodextei to protocol verion
        //mou stelnei to id tou client sti morifi: I<plength><id>

        //lamvanei ta akoloutha:
        //<1 byte id length><id><1 byte cachedID lenght><cached id>
        std::vector<char> cmdIDdataBuff;
        _receive(activeSocket, cmdIDdataBuff); // <--------------------- to client id pou mou esteile o server

        //lamvanw to megethos pou exei to id kai to id
        int idLength = bytesToInt(cmdIDdataBuff, 0, 1);
        myID = std::vector<char> (cmdIDdataBuff.begin()+1, cmdIDdataBuff.begin()+1+idLength);

        //lamvanw to megethos pou exei to cachedID kai to cachedID
        int cached_idLength = bytesToInt(cmdIDdataBuff, idLength+1, 1);
        cachedID = std::vector<char> (cmdIDdataBuff.begin()+1+1+idLength, cmdIDdataBuff.begin()+1+1+idLength+cached_idLength);

        std::cout << "Client ID received from server: " << myID.data() << std::endl;
        //std::vector<char> emtpyv;
        setConnectionState(connectionState::connectedWithProxy);
        emit sig_messageReceived(this, MSG_ID, myID);

        if (password.size() == 0){ //efoson den exei dimiourgithei password, to dimiourgw
            password = generateRandomPassword(PASSWORD_LENGTH);
            emit sig_messageReceived(this, MSG_LOCAL_PASSWORD_GENERATED);
        }
    } // CMD_ID

    else if(command == CMD_CONNECT)
    {
        std::vector<char> remote_client_idbuff;
        _receive(activeSocket, remote_client_idbuff); // <-----------to remote client id pou prokalese to connect

        //o typos tis syndesis
        std::vector<char> connMsgType(remote_client_idbuff.begin(), remote_client_idbuff.begin() + 1);
        connectMessageType enumconnMsgType = (connectMessageType) bytesToInt(connMsgType);

        //to remote computer os
        //o typos tis syndesis
        std::vector<char> connRemoteComputerOS(remote_client_idbuff.begin() + 1, remote_client_idbuff.begin() + 2);
        setRemoteComputerOS((OS)bytesToInt(connRemoteComputerOS));

        if(enumconnMsgType == connectMessageType::proxy){
            //lamvanw to proto byte poy mas deixnei to size tou ID tou apomakrismenou ypologisti
            std::vector<char> idSize(remote_client_idbuff.begin() + 2, remote_client_idbuff.begin() + 3);
            int iIDSize = bytesToInt(idSize);

            //efou gnwrizoume to length tou id, lamvanoume to id sto vector
            std::vector<char> vremoteID(remote_client_idbuff.begin() + 3, remote_client_idbuff.begin() + 3 + iIDSize);

            //sti nynexeia lamvanoume to password size
            std::vector<char> pwdSize(remote_client_idbuff.begin() + 3 + iIDSize, remote_client_idbuff.begin() + 3 + iIDSize + 1);
            int ipwdSize = bytesToInt(pwdSize);

            //lamvanw to password
            std::vector<char> vpassword(remote_client_idbuff.begin() + 3 + iIDSize + 1, remote_client_idbuff.begin() + 3 + iIDSize + 1 + ipwdSize);

            //lamvmanw tin ip address to client poy thelei na syndethei
            std::vector<char> vclientIP(remote_client_idbuff.begin() + 3 + iIDSize + 1 + ipwdSize, remote_client_idbuff.end());
            unsigned int iclientIP = bytesToInt(vclientIP);

            //elegxw ean exei ginei ban i IP
            if (isIPBannedForWrongPasswords(iclientIP, activeSocket))
                return;

            //elegxw ean to password pou stalthike einai to idio me to password pou exei o client edw
            std::string passwdQuestion(vpassword.begin(), vpassword.end());
            if (passwdQuestion == this->password){
//#ifdef Q_OS_MAC
//        disableAppNap();
//#endif
                //to password pou stalthike einai swsto
                std::cout << "Client connection accepted. Remote Client ID: "<< remote_client_idbuff.data() << std::endl;

                //kanw reset to password protection gia thn sygekrimenh ip
                resetWrongPasswordIPProtection(iclientIP);

                setConnectionState(connectedWithOtherAsServer);

                _sendmsg(activeSocket, CMD_ACCEPT, vremoteID);//>-------------stelnw sto server oti egine accept tou Andama request, stelnontas to remote id
                //std::vector<char> emtpyv;
                emit sig_messageReceived(this, MSG_REMOTE_CLIENT_ACCEPTED, remote_client_idbuff);
            } else {
                //to password pou stalthike einai lathos
                //lamvanw apo to vector tin ip tou client
                addWrongPasswordIPProtection(iclientIP, activeSocket);
            }
        } // if connectMessageType::proxy
    } // CMD_CONNECT

    // to password pou stalthike itan lathos
    else if(command == CMD_CONNECT_PASSWORD_NOT_CORRECT){
        emit sig_messageReceived(this, MSG_CONNECT_PASSWORD_NOT_CORRECT);
    }

    // egine ban tis IP logw epanalamvanomenwn
    // prospathiwn syndesis me lathos password
    else if(command == CMD_BAN_IP_WRONG_PWD){
        emit sig_messageReceived(this, MSG_BAN_IP_WRONG_PWD);
    }

    // lipsi warning gia tis ypoloipomenes prospathies
    // syndesis dinontas lathos password
    else if(command == CMD_WARNING_BAN_IP_WRONG_PWD){
        //lamvanw tis ypoloipomenes prospathies
        std::vector<char> remain_tries_buff(1);
        _receivePlain(activeSocket, remain_tries_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_WARNING_BAN_IP_WRONG_PWD, remain_tries_buff);
    }

    // to id pou zitithike syndesi den yparxei
    else if(command == CMD_CONNECT_ID_NOT_FOUND){
        std::vector<char> emtpyv;
        emit sig_messageReceived(this, MSG_CONNECT_ID_NOT_FOUND, emtpyv);
    } //CMD_CONNECT_ID_NOT_FOUND

    //egine apodoxi tis syndesis pros to remote computer
    else if(command == CMD_ACCEPT)
    {
        std::vector<char> remote_client_id_buff;
        _receive(activeSocket, remote_client_id_buff); //<-----------to remote client id

        setConnectionState(connectionState::connectedWithOtherAsClient);

        //if(remotePort != 0){
        //    isClientConnected = true;
        //}

        //std::vector<char> emptyv;
        emit sig_messageReceived(this, MSG_CONNECTION_ACCEPTED, remote_client_id_buff);
    }

    else if(command == CMD_DISCONNECT_FROM_REMOTE_COMPUTER)
    {
        setConnectionState(connectionState::connectedWithProxy);
        emit sig_messageReceived(this, MSG_REMOTE_COMPUTER_DISCONNECTED);

//#ifdef Q_OS_MAC
//        enableAppNap();
//#endif
    }

    else if(command == CMD_REQUEST_SCREENSHOT)
    {
        //qDebug("Lipsi aitimatos apostolis screenshot.");

        std::vector<char> emptyv;
        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_SCREENSHOT_REQUEST, emptyv);
    } // CMD_REQUEST_SREENSHOT

    else if(command == CMD_REQUEST_SCREENSHOT_DIFF)
    {
        //lamvanw to id tou request
        std::vector<char> screenshot_diff_id_data_buff;
        _receive(activeSocket, screenshot_diff_id_data_buff);
        std::string rid(screenshot_diff_id_data_buff.begin(), screenshot_diff_id_data_buff.end());

        //qDebug ("4. CMD_REQUEST_SCREENSHOT_DIFF ID: %s. Lipsi aitimatos apostolis screenshot diff. Will emit signal from protocol to ui. Bytes recv: %i",rid.c_str(),bytes_received);
        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_SCREENSHOT_DIFF_REQUEST, screenshot_diff_id_data_buff);
    } // CMD_REQUEST_SCREENSHOT_DIFF

    else if(command == CMD_SCREENSHOT)
    {
        RequestScreenshotDiff();
        std::vector<char> screenshot_data_buff;
        _receive(activeSocket, screenshot_data_buff);
        //qDebug("Screenshot received! tΒotal bytes: %i",bytes_received);
        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_SCREENSHOT, screenshot_data_buff);
    }//CMD_SCREENSHOT

    else if(command == CMD_SCREENSHOT_DIFF)
    {
        RequestScreenshotDiff();
        std::vector<char> screenshot_diff_data_buff;
        _receive(activeSocket, screenshot_diff_data_buff);
        //qDebug("DS.1 Diff screenshot received! Total bytes: %i. Tha ginei emit sto UI.",bytes_received);

        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_SCREENSHOT_DIFF, screenshot_diff_data_buff);
    }//CMD_SCREENSHOT_DIFF

    else if(command == CMD_MOUSE)
    {
        std::vector<char> mouse_data_buff(10);
        _receivePlain(activeSocket, mouse_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_MOUSE, mouse_data_buff);
    }// send mouse

    else if(command == CMD_KEYBOARD)
    {
        std::vector<char> keyboard_data_buff;
        _receive(activeSocket, keyboard_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_KEYBOARD, keyboard_data_buff);
    }// CMD_KEYBOARD

    //o server esteile to id kai remote upnp port tou ypologisti
    //opote o client mporei na dokimasei na syndethei apeftheias ston remote client
    else if(command == CMD_P2P_REMOTE_CLIENT_UPNP_PORT){
        //i morfi tou minimatos pou stelnetai ston client einai:
        // | 1 byte command | 4 bytes message length | +
        // | 1 bytes remote client ID length | x bytes remote client ID | +
        // | 2 bytes UPnP port |
        std::vector<char> buffsend_remote_p2p_client_id_and_port;
        _receive(activeSocket, buffsend_remote_p2p_client_id_and_port);

        //lamvanw to remote client ID kai to ID
        int remoteClientIDLength = bytesToInt(buffsend_remote_p2p_client_id_and_port, 0, 1);
        std::string remoteClientID;
        remoteClientID.insert(remoteClientID.begin(), buffsend_remote_p2p_client_id_and_port.begin() + 1, buffsend_remote_p2p_client_id_and_port.begin() + 1 + remoteClientIDLength);

        std::cout << "Remote client ID:" << remoteClientID << ", UPnP port:" << std::endl;
        //lamvana to upnp port tou remote client

    } //CMD_P2P_REMOTE_CLIENT_UPNP_PORT

    else if(command == CMD_WARNING_BAN_IP_WRONG_ID)
    {
        std::vector<char> warn_remaining_tries_data_buff(1);
        _receivePlain(activeSocket, warn_remaining_tries_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_WARNING_BAN_IP_WRONG_ID, warn_remaining_tries_data_buff);
    }// CMD_WARNING_BAN_IP_WRONG_ID

    else if(command == CMD_BAN_IP_WRONG_ID)
    {
        std::vector<char> ban_remaining_sec_data_buff(4);
        _receivePlain(activeSocket, ban_remaining_sec_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(this, MSG_BAN_IP_WRONG_ID, ban_remaining_sec_data_buff);
    }// CMD_BAN_IP_WRONG_ID

    else if(command == CMD_ERROR_APP_VERSION_NOT_ACCEPTED)
    {
        std::vector<char> vurl;
        _receive(activeSocket, vurl);
        emit sig_messageReceived(this, MSG_ERROR_APP_VERSION_NOT_ACCEPTED, vurl);
    } // CMD_ERROR_APP_VERSION_NOT_ACCEPTED

    else
    {
        qDebug ("---->> TO COMMAND DEN VRETHIKE : %s", command);
    }
}


#ifdef WIN32
void clientServerProtocol::cleanup(const SOCKET socketfd)
#else
void clientServerProtocol::cleanup(const int socketfd)
#endif
{
    /*
    try
    {
    #ifdef WIN32
    closesocket(socketfd);
    #else
    close(socketfd);
    #endif
    }
    catch ( ... ){}
    */
}

void clientServerProtocol::sendHeartBeat()
{
    std::cout << ">> will send heart beat to proxy server" << std::endl;
    _sendmsgPlain(activeSocket, CMD_HEART_BEAT);
}

void clientServerProtocol::setRemoteComputerOS(helperfuncs::OS os)
{
    _remoteComputerOS = os;
}
OS clientServerProtocol::getRemoteComputerOS()
{
    return _remoteComputerOS;
}

void clientServerProtocol::sendKeyboard(int portableVKey, int portableModifiers, int keyEvent)
{
    //4 bytes portableVKey
    //1 byte portableModifiers
    //1 byte keyEvent (1 press, 2 release)
    std::vector<char> msg(6);

    std::vector<char> _portableVKey(4);
    intToBytes(portableVKey, _portableVKey);
    msg[0] = _portableVKey[0];
    msg[1] = _portableVKey[1];
    msg[2] = _portableVKey[2];
    msg[3] = _portableVKey[3];

    std::vector<char> _portableModifiers(1);
    intToBytes(portableModifiers, _portableModifiers);
    msg[4] = _portableModifiers[0];

    std::vector<char> _keyEvent(1);
    intToBytes(keyEvent, _keyEvent);
    msg[5] = _keyEvent[0];

    openssl_aes::secure_string ctext;
    openssl_aes myaes(EVP_aes_256_cbc());
    openssl_aes::byte key[openssl_aes::KEY_SIZE_256_BITS] = {1,2,3,4,5,6,7,8,   1,2,3,4,5,6,7,8,   1,2,3,4,5,6,7,8,   1,2,3,4,5,6,7,8};
    openssl_aes::byte iv[openssl_aes::BLOCK_SIZE_128_BITS] = {1,2,3,4,5,6,7,8,   1,2,3,4,5,6,7,8};
    openssl_aes::secure_string ptext (msg.begin(),msg.end());
    myaes.aes_256_cbc_encrypt(key, iv, ptext, ctext);
    std::vector<char> vect_ciptext(ctext.begin(), ctext.end());

    _sendmsg(activeSocket, CMD_KEYBOARD, vect_ciptext);

    //_sendmsgPlain(activeSocket, CMD_KEYBOARD, msg);
}

void clientServerProtocol::sendMouse(int x, int y, int button, int mouseEvent, int wheelDelta, int wheelDeltaSign, int wheelOrientation)
{
    //x 2 bytes
    //y 2 bytes
    //button 1 byte (0 kanena, 1=left, 2=middle, 4 right...)
    //buttonState 1 byte (0 aprosdioristo, 1 press, 2 release)
    //wheel 2 bytes

    std::vector<char> msg(10);

    std::vector<char> _xpos(2);
    intToBytes(x, _xpos);
    msg[0] = _xpos[0];
    msg[1] = _xpos[1];

    std::vector<char> _ypos(2);
    intToBytes(y, _ypos);
    msg[2] = _ypos[0];
    msg[3] = _ypos[1];

    std::vector<char> _btn(1);
    intToBytes(button, _btn);
    msg[4] = _btn[0];

    std::vector<char> _mouseEvent(1);
    intToBytes(mouseEvent, _mouseEvent);
    msg[5] = _mouseEvent[0];

    std::vector<char> _wheelDelta(2);
    intToBytes(wheelDelta, _wheelDelta);
    msg[6] = _wheelDelta[0];
    msg[7] = _wheelDelta[1];

    std::vector<char> _wheelDeltaSign(1);
    intToBytes(wheelDeltaSign, _wheelDeltaSign);
    msg[8] = _wheelDeltaSign[0];

    std::vector<char> _wheelOrientation(1);
    intToBytes(wheelOrientation, _wheelOrientation);
    msg[9] = _wheelOrientation[0];

    _sendmsgPlain(activeSocket, CMD_MOUSE, msg);
}

//stelnei ston server tin porta poy o client akouei gia eiserxomenes P2P syndeseis
void clientServerProtocol::sendUPnPPort(int port)
{
    //2 bytes  > port number 0-65535
    std::vector<char> msg(2);

    std::vector<char> _msg_bytes(2);
    intToBytes(port, _msg_bytes);
    msg[0] = _msg_bytes[0];
    msg[1] = _msg_bytes[1];

    _sendmsg(activeSocket, CMD_P2P_CLIENT_UPNP_PORT, _msg_bytes);
}

void clientServerProtocol::setConnectionState(connectionState state)
{
    std::lock_guard<std::mutex> lock(connection_state_mutex);
    m_connection_state = state;
}

connectionState clientServerProtocol::getConnectionState()
{
    std::lock_guard<std::mutex> lock(connection_state_mutex);
    return m_connection_state;
}

void clientServerProtocol::sendDisconnectFromRemoteComputer()
{
    //1 byte
     setConnectionState(connectionState::connectedWithProxy);
    _sendmsgPlain(activeSocket, CMD_DISCONNECT_FROM_REMOTE_COMPUTER);
}

//aitisi connect se allo ypologisti
void clientServerProtocol::Connect(const std::vector<char> &remoteID, const std::vector<char> &remotePassword){
    //_sendmsg(socketfd,CMD_CONNECT,remoteID);//>---------------------aitisi connect se allo ypologisti
    if (myID == remoteID){
        emit sig_messageReceived(this, MSG_ERROR_CANNOT_CONNECT_SAME_ID);
        return;
    }

    std::vector<char> data;
    createConnectCommandData(data, remoteID, remotePassword);
    _sendmsg(activeSocket, CMD_CONNECT, data);
}

//aitisi connect se allo ypologisti apefthias
void clientServerProtocol::ConnectP2P(const std::vector<char> remotePassword){
    std::vector<char> data;
    createConnectP2PCommandData(data, remotePassword);
    _sendmsg(activeSocket, CMD_P2P_CONNECT, data);
}

void clientServerProtocol::RequestScreenshot(){
    _sendmsgPlain(activeSocket, CMD_REQUEST_SCREENSHOT);
}
void clientServerProtocol::RequestScreenshotDiff(){
    //diffRequestCounter++;
    std::string cnt = std::to_string(0);
    std::vector<char> msg(cnt.begin(), cnt.end());
    _sendmsg(activeSocket, CMD_REQUEST_SCREENSHOT_DIFF, msg);
    //qDebug ("Egine apostoli aitimatos CMD_REQUEST_SCREENSHOT_DIFF me ID: %s kai perimeno to screenshot diff.",cnt.c_str());
    //_sendmsg(localsocket,CMD_REQUEST_SCREENSHOT_DIFF,1);
}

std::string clientServerProtocol::generateRandomPassword(int length)
{
    //static std::string charset = "123456789123456789abcdefgkmnprstxz123456789123456789"; //52
    std::string result;
    result.resize(length);

    for (int i = 0; i < length; i++){
        int d = distribution(mt);
        result[i] = passwordCharset[d];
    }

    return result;
}

#ifdef WIN32
bool clientServerProtocol::isIPBannedForWrongPasswords(in_addr_t clientIP, SOCKET socketfd)
#else
bool clientServerProtocol::isIPBannedForWrongPasswords(in_addr_t clientIP, int socketfd)
#endif
{
    std::lock_guard<std::mutex> lock(protect_password_mutex);

    if (protect_password.count(clientIP) == 0){
        //den vrethike i IP, opote den exei ginei ban
        return false;
    }

    //vrethike clientIP
    //Elegxw ean exei ginei ban h oxi akoma
    PasswordProtection & pwprotect = protect_password[clientIP];
    if (pwprotect.wrongIDCounter >= MAX_WRONG_PWD_TRIES){
        // *** BAN ***
        // exei ginei ban.
        // enimerwnw kai ton client
        _sendmsgPlain(socketfd, CMD_BAN_IP_WRONG_PWD);
        return true;
    }

    //den exei ginei ban
    return false;
}


void clientServerProtocol::resetWrongPasswordIPProtection(in_addr_t clientIP)
{
    std::lock_guard<std::mutex> lock(protect_password_mutex);

    if (protect_password.count(clientIP) != 0){
        //vrethike h clientIP opote thetw tis lathos apopeires
        //password se 0
        PasswordProtection & pwprotect = protect_password[clientIP];
        pwprotect.wrongIDCounter = 0;
    }
}

#ifdef WIN32
bool clientServerProtocol::addWrongPasswordIPProtection(in_addr_t clientIP, SOCKET socketfd)
#else
bool clientServerProtocol::addWrongPasswordIPProtection(in_addr_t clientIP, int socketfd)
#endif
{
    std::lock_guard<std::mutex> lock(protect_password_mutex);

    if (protect_password.count(clientIP) == 0){
        //den vrethike to clientIP
        //opote kataxwrw tin ip pou ekane lathos password
        //kai arxikopoiw tis times
        PasswordProtection pwprotect;
        pwprotect.wrongIDCounter = 1;
        protect_password[clientIP] = pwprotect;

        //enimerwnw kai ton client pou ekane to aitima
        _sendmsgPlain(socketfd, CMD_CONNECT_PASSWORD_NOT_CORRECT);

        return true;
    }
    else {
        //vrethike clientIP.
        //Elegxw ean prepei na ginei ban h oxi akoma
        PasswordProtection & pwprotect = protect_password[clientIP];
        pwprotect.wrongIDCounter++;

        //Elegxos ean prepei na ginei ban tis IP
        if (pwprotect.wrongIDCounter >= MAX_WRONG_PWD_TRIES){
            // *** BAN ***
            //kseperase to megisto orio lathos prospathiwn
            //opote kanw ban tin ip kai
            //enimerwnw ton client oti exei ginei ban
            _sendmsgPlain(socketfd, CMD_BAN_IP_WRONG_PWD);
            return false;
        }
        else {
            //den exei ginei ban akoma opote
            //eksetazw ean prepei na apostalei warning h oxi
            if (pwprotect.wrongIDCounter >= MAX_WRONG_PWD_TRIES_WARNING){
                //apostoli warning
                int remain = MAX_WRONG_PWD_TRIES - pwprotect.wrongIDCounter;
                std::vector<char> vremain(1);
                intToBytes(remain, vremain);
                _sendmsgPlain(socketfd, CMD_WARNING_BAN_IP_WRONG_PWD, vremain);
                return true;
            }
            else {
                //den exoume ftasei akomi sta lathi pou
                //prepei na emfanistei warning opote
                //ola ok

                //enimerwnw kai ton client pou ekane to aitima
                _sendmsgPlain(socketfd, CMD_CONNECT_PASSWORD_NOT_CORRECT);

                return true;
            }
        }
    }
}

/* Gia na syndethei enas ypologistis se ena allo, prepei na steilei to ID tou wste
 * na kserei o allos poios einai (xrisimo se periptwsi poy ylopoiithei allagi plevras)
 * kai to password tou allou ypologisti wste na ginei authentication.
 * H morfi tou command exei ws ekshs:
 * //| 1 byte command | 4 byte msg payload | 1 byte pou prosdiorizei to ID length_1 | ID me bytes to length_1 | password me bytes oso payload - length_1 |
 *| 1 byte command | 4 byte msg payload | 1 byte msg type (0=proxy, 1=direct) | 1 byte pou prosdiorizei to ID length_1 | ID me bytes to length_1 | 1 byte password length_2 | password me bytes to length_2 | IP 4 oso payload - length_1 - length_2 - 3 |
 * To pedio tis IP to prosthetei o proxy
 */
void clientServerProtocol::createConnectCommandData(std::vector<char> &all_data, const std::vector<char> &remoteComputerID, const std::vector<char> &remoteComputerPassword)
{
    //1 byte msg type
    std::vector<char> connMsgTypeSize(1);
    intToBytes(connectMessageType::proxy_or_p2p, connMsgTypeSize);
    all_data.insert(all_data.begin(), connMsgTypeSize[0]); // size tou connMsgTypeSize

    //1 byte OS
    std::vector<char> connOSSize(1);
    intToBytes(getOS(), connOSSize);
    all_data.insert(all_data.end(), connOSSize[0]); // to trexon leitourgiko

    //ypologismos ID size kai kataxwrisi ID
    std::vector<char> idSize(1);
    intToBytes(remoteComputerID.size(), idSize);
    all_data.insert(all_data.end(), idSize[0]); // size tou ID
    all_data.insert(all_data.end(), remoteComputerID.begin(), remoteComputerID.end()); // to ID

    //ypologismos PWD size kai kataxwrisi PWD
    std::vector<char> pwdSize(1);
    intToBytes(remoteComputerPassword.size(), pwdSize);
    all_data.insert(all_data.end(), pwdSize[0]); // size tou PWD
    all_data.insert(all_data.end(), remoteComputerPassword.begin(), remoteComputerPassword.end()); // to PWD
}

/* Gia na syndethei enas ypologistis se ena allo, prepei na steilei to ID tou wste
 * na kserei o allos poios einai (xrisimo se periptwsi poy ylopoiithei allagi plevras)
 * kai to password tou allou ypologisti wste na ginei authentication.
 * H morfi tou command exei ws ekshs:
 *| 1 byte command | 4 byte msg payload | 1 byte pou prosdiorizei to OS | 1 byte password length | password me bytes to length |
 */
void clientServerProtocol::createConnectP2PCommandData(std::vector<char> &all_data, const std::vector<char> remoteComputerPassword)
{
    //1 byte OS
    std::vector<char> connOSSize(1);
    intToBytes(getOS(), connOSSize);
    all_data.insert(all_data.end(), connOSSize[0]); // to trexon leitourgiko

    //ypologismos PWD size kai kataxwrisi PWD
    std::vector<char> pwdSize(1);
    intToBytes(remoteComputerPassword.size(), pwdSize);
    all_data.insert(all_data.end(), pwdSize[0]); // size tou PWD
    all_data.insert(all_data.end(), remoteComputerPassword.begin(), remoteComputerPassword.end()); // to PWD
}
