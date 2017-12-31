/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (yiannis -{at}- grbytes.com)
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

#include "clientserver.h"
#include "../Shared/AndamaHeaders/shared_constants.h"
#include "../Shared/AndamaHeaders/byte_functions.h"
#include "../Shared/AndamaHeaders/shared_enums.h"
#include "../Shared/AndamaHeaders/exception_helper.h"
#include "../Shared/AndamaHeaders/socket_functions.h"
#include "../Shared/Cryptography/openssl_aes.h"

std::vector<char> myID;
static std::mutex protect_password_mutex;
static std::map<in_addr_t,PasswordProtection> protect_password;//krataei tis ip pou exoun kanei apopeira syndesis me password pou einai lathos
static std::mutex sendmutex;
static std::string remotepassword;

clientserver::clientserver()
{
    setRemoteComputerOS(OS::Unknown);
    remotePort=0;
}

void clientserver::run(void)
{
   this->start_protocol();
}


#ifdef WIN32
void clientserver::setActiveSocket(const SOCKET socket)
#else
void clientserver::setActiveSocket(const int socket)
#endif
{
    this->_activeSocket = socket;
}

#ifdef WIN32
SOCKET clientserver::getActiveSocket()
#else
int clientserver::getActiveSocket()
#endif
{
    return this->_activeSocket;
}

void clientserver::error(const char *msg)
{
    std::cout << "ERROR " << msg <<  "\n";
    perror(msg);
    //exit(0);
}

#ifdef WIN32
void clientserver::cleanup(const SOCKET socketfd)
#else
void clientserver::cleanup(const int socketfd)
#endif
{
    static_cast<void>(socketfd);
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

void clientserver::sendHeartBeat()
{
    _sendmsgPlain(this->getActiveSocket(), CMD_HEART_BEAT);
}

void clientserver::setRemoteComputerOS(OS os)
{
    _remoteComputerOS = os;
}
OS clientserver::getRemoteComputerOS()
{
    return _remoteComputerOS;
}

void clientserver::sendKeyboard(int portableVKey, int portableModifiers, int keyEvent)
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

    _sendmsg(this->getActiveSocket(), CMD_KEYBOARD, vect_ciptext);
    //_sendmsgPlain(this->getActiveSocket(), CMD_KEYBOARD, msg);
}

void clientserver::setRemotePassword(std::string password)
{
    remotepassword = password;
    qDebug() << ">>>>>>>>>>>>>>  setRemotePassword:" << QString(remotepassword.c_str());
}

void clientserver::sendMouse(int x, int y, int button, int mouseEvent, int wheelDelta, int wheelDeltaSign, int wheelOrientation)
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

    _sendmsgPlain(this->getActiveSocket(), CMD_MOUSE, msg);
}

void clientserver::setConnectionState(connectionState state)
{
    std::lock_guard<std::mutex> lock(connection_state_mutex);
    m_connection_state = state;
}

connectionState clientserver::getConnectionState()
{
    std::lock_guard<std::mutex> lock(connection_state_mutex);
    return m_connection_state;
}

void clientserver::sendDisconnectFromRemoteComputer()
{
    //1 byte
     setConnectionState(connectionState::connectedWithProxy);
    _sendmsgPlain(this->getActiveSocket(), CMD_DISCONNECT_FROM_REMOTE_COMPUTER);

#ifdef WIN32
     closesocket(this->getActiveSocket());
#else
     shutdown(this->getActiveSocket(), 2);
     //close(this->getActiveSocket());
#endif

}

//aitisi connect se allo ypologisti
void clientserver::Connect(const std::vector<char> &remoteID, const std::vector<char> &remotePassword){
    //_sendmsg(socketfd,CMD_CONNECT,remoteID);//>---------------------aitisi connect se allo ypologisti
    if (myID == remoteID){
        emit sig_messageReceived(NULL, MSG_ERROR_CANNOT_CONNECT_SAME_ID);
        return;
    }

    std::vector<char> data;
    createConnectCommandData(data, remoteID, remotePassword);
    _sendmsg(this->getActiveSocket(), CMD_CONNECT, data);
}

//aitisi connect se allo ypologisti apefthias
void clientserver::ConnectP2P(const std::vector<char> remotePassword){
    std::vector<char> data;
    createConnectP2PCommandData(data, remotePassword);
    _sendmsg(this->getActiveSocket(), CMD_P2P_CONNECT, data);
}

void clientserver::RequestScreenshot(){
    _sendmsgPlain(this->getActiveSocket(), CMD_REQUEST_SCREENSHOT);
}
void clientserver::RequestScreenshotDiff(){
    diffRequestCounter++;
    std::string cnt = std::to_string(diffRequestCounter);
    std::vector<char> msg(cnt.begin(), cnt.end());
    _sendmsg(this->getActiveSocket(), CMD_REQUEST_SCREENSHOT_DIFF, msg);
    //qDebug ("Egine apostoli aitimatos CMD_REQUEST_SCREENSHOT_DIFF me ID: %s kai perimeno to screenshot diff.",cnt.c_str());
    //_sendmsg(localsocket,CMD_REQUEST_SCREENSHOT_DIFF,1);
}

std::string clientserver::generateRandomPassword(int length)
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
bool clientserver::isIPBannedForWrongPasswords(in_addr_t clientIP, SOCKET socketfd)
#else
bool clientserver::isIPBannedForWrongPasswords(in_addr_t clientIP, int socketfd)
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
    if (pwprotect.wrongIDCounter >= clientserver::MAX_WRONG_PWD_TRIES){
        // *** BAN ***
        // exei ginei ban.
        // enimerwnw kai ton client
        _sendmsgPlain(socketfd, CMD_BAN_IP_WRONG_PWD);
        return true;
    }

    //den exei ginei ban
    return false;
}


void clientserver::resetWrongPasswordIPProtection(in_addr_t clientIP)
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
bool clientserver::addWrongPasswordIPProtection(in_addr_t clientIP, SOCKET socketfd)
#else
bool clientserver::addWrongPasswordIPProtection(in_addr_t clientIP, int socketfd)
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
        if (pwprotect.wrongIDCounter >= clientserver::MAX_WRONG_PWD_TRIES){
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
            if (pwprotect.wrongIDCounter >= clientserver::MAX_WRONG_PWD_TRIES_WARNING){
                //apostoli warning
                int remain = clientserver::MAX_WRONG_PWD_TRIES - pwprotect.wrongIDCounter;
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
void clientserver::createConnectCommandData(std::vector<char> &all_data, const std::vector<char> &remoteComputerID, const std::vector<char> &remoteComputerPassword)
{
    //1 byte msg type
    std::vector<char> connMsgTypeSize(1);
    intToBytes(connectMessageType::proxy, connMsgTypeSize);
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
void clientserver::createConnectP2PCommandData(std::vector<char> &all_data, const std::vector<char> remoteComputerPassword)
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

bool clientserver::proccesCommand(const std::array<char, 1> &command){
    //qDebug("3. Inside proccessCommand. Checking received command: %s",command.data());

    if(command == CMD_PROTOCOL)
    {
        //std::string ww ("yian");
        //cachedID.insert(cachedID.begin(),ww.begin(),ww.end());

        //o server stelnei tin version toy protocol pou ypostirizei
        //sti morfi "P<major><minor>" opou major,minor apo ena byte.
        //synolo command 3 bytes
        std::vector<char> protocolbuff(2);
        _receivePlain(this->getActiveSocket(), protocolbuff);
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
        _sendmsg(this->getActiveSocket(), CMD_PROTOCOL, client_appver_and_protocolver);
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
        _receive(this->getActiveSocket(), cmdIDdataBuff); // <--------------------- to client id pou mou esteile o server

        //lamvanw to megethos pou exei to id kai to id
        int idLength = bytesToInt(cmdIDdataBuff, 0, 1);
        myID = std::vector<char> (cmdIDdataBuff.begin()+1, cmdIDdataBuff.begin()+1+idLength);

        //lamvanw to megethos pou exei to cachedID kai to cachedID
        int cached_idLength = bytesToInt(cmdIDdataBuff, idLength+1, 1);
        cachedID = std::vector<char> (cmdIDdataBuff.begin()+1+1+idLength, cmdIDdataBuff.begin()+1+1+idLength+cached_idLength);

        std::cout << "Client ID received from server: " << myID.data() << std::endl;
        //std::vector<char> emtpyv;
        setConnectionState(connectionState::connectedWithProxy);
        emit sig_messageReceived(NULL, MSG_ID, myID);

        if (password.size() == 0){ //efoson den exei dimiourgithei password, to dimiourgw
            password = generateRandomPassword(PASSWORD_LENGTH);
            emit sig_messageReceived(NULL, MSG_LOCAL_PASSWORD_GENERATED);
        }
    } // CMD_ID

    else if(command == CMD_CONNECT)
    {
        std::vector<char> remote_client_idbuff;
        _receive(this->getActiveSocket(), remote_client_idbuff); // <-----------to remote client id pou prokalese to connect

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
            if (isIPBannedForWrongPasswords(iclientIP, this->getActiveSocket())){
                return true;
            }

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

                _sendmsg(this->getActiveSocket(), CMD_ACCEPT, vremoteID);//>-------------stelnw sto server oti egine accept tou Andama request, stelnontas to remote id
                //std::vector<char> emtpyv;
                emit sig_messageReceived(NULL, MSG_REMOTE_CLIENT_ACCEPTED, remote_client_idbuff);
            } else {
                //to password pou stalthike einai lathos
                //lamvanw apo to vector tin ip tou client
                addWrongPasswordIPProtection(iclientIP, this->getActiveSocket());
                return true;

            }
        } // if connectMessageType::proxy
    } // CMD_CONNECT

    // to password pou stalthike itan lathos
    else if(command == CMD_CONNECT_PASSWORD_NOT_CORRECT){
        emit sig_messageReceived(NULL, MSG_CONNECT_PASSWORD_NOT_CORRECT);
    }

    // egine ban tis IP logw epanalamvanomenwn
    // prospathiwn syndesis me lathos password
    else if(command == CMD_BAN_IP_WRONG_PWD){
        emit sig_messageReceived(NULL, MSG_BAN_IP_WRONG_PWD);
    }

    // lipsi warning gia tis ypoloipomenes prospathies
    // syndesis dinontas lathos password
    else if(command == CMD_WARNING_BAN_IP_WRONG_PWD){
        //lamvanw tis ypoloipomenes prospathies
        std::vector<char> remain_tries_buff(1);
        _receivePlain(this->getActiveSocket(), remain_tries_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_WARNING_BAN_IP_WRONG_PWD, remain_tries_buff);
    }

    // to id pou zitithike syndesi den yparxei
    else if(command == CMD_CONNECT_ID_NOT_FOUND){
        std::vector<char> emtpyv;
        emit sig_messageReceived(NULL, MSG_CONNECT_ID_NOT_FOUND, emtpyv);
    } //CMD_CONNECT_ID_NOT_FOUND

    //egine apodoxi tis syndesis pros to remote computer
    else if(command == CMD_ACCEPT)
    {
        std::vector<char> remote_client_id_buff;
        _receive(this->getActiveSocket(), remote_client_id_buff); //<-----------to remote client id

        setConnectionState(connectionState::connectedWithOtherAsClient);

        if(remotePort != 0){
            isClientConnected = true;
        }

        //std::vector<char> emptyv;
        emit sig_messageReceived(NULL, MSG_CONNECTION_ACCEPTED, remote_client_id_buff);
    }

    else if(command == CMD_DISCONNECT_FROM_REMOTE_COMPUTER)
    {
        setConnectionState(connectionState::connectedWithProxy);
        emit sig_messageReceived(NULL, MSG_REMOTE_COMPUTER_DISCONNECTED);
        return false;

//#ifdef Q_OS_MAC
//        enableAppNap();
//#endif
    }

    else if(command == CMD_REQUEST_SCREENSHOT)
    {
        //qDebug("Lipsi aitimatos apostolis screenshot.");

        std::vector<char> emptyv;
        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_SCREENSHOT_REQUEST, emptyv);
    } // CMD_REQUEST_SREENSHOT

    else if(command == CMD_REQUEST_SCREENSHOT_DIFF)
    {
        //lamvanw to id tou request
        std::vector<char> screenshot_diff_id_data_buff;
        _receive(this->getActiveSocket(), screenshot_diff_id_data_buff);
        //std::string rid(screenshot_diff_id_data_buff.begin(), screenshot_diff_id_data_buff.end());
        //qDebug ("4. CMD_REQUEST_SCREENSHOT_DIFF ID: %s. Lipsi aitimatos apostolis screenshot diff. Will emit signal from protocol to ui. Bytes recv: %i",rid.c_str(),bytes_received);
        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_SCREENSHOT_DIFF_REQUEST, screenshot_diff_id_data_buff);
    } // CMD_REQUEST_SCREENSHOT_DIFF

    else if(command == CMD_SCREENSHOT)
    {
        RequestScreenshotDiff();
        std::vector<char> screenshot_data_buff;
        _receive(this->getActiveSocket(), screenshot_data_buff);
        //qDebug("Screenshot received! tΒotal bytes: %i",bytes_received);
        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_SCREENSHOT, screenshot_data_buff);
    }//CMD_SCREENSHOT

    else if(command == CMD_SCREENSHOT_DIFF)
    {
        RequestScreenshotDiff();
        std::vector<char> screenshot_diff_data_buff;
        _receive(this->getActiveSocket(), screenshot_diff_data_buff);
        //qDebug("DS.1 Diff screenshot received! Total bytes: %i. Tha ginei emit sto UI.",bytes_received);

        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_SCREENSHOT_DIFF, screenshot_diff_data_buff);
    }//CMD_SCREENSHOT_DIFF

    else if(command == CMD_MOUSE)
    {
        std::vector<char> mouse_data_buff(10);
        _receivePlain(this->getActiveSocket(), mouse_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_MOUSE, mouse_data_buff);
    }// send mouse

    else if(command == CMD_KEYBOARD)
    {
        std::vector<char> keyboard_data_buff(6);
        _receivePlain(this->getActiveSocket(), keyboard_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_KEYBOARD, keyboard_data_buff);
    }// CMD_KEYBOARD

    else if(command == CMD_WARNING_BAN_IP_WRONG_ID)
    {
        std::vector<char> warn_remaining_tries_data_buff(1);
        _receivePlain(this->getActiveSocket(), warn_remaining_tries_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_WARNING_BAN_IP_WRONG_ID, warn_remaining_tries_data_buff);
    }// CMD_WARNING_BAN_IP_WRONG_ID

    else if(command == CMD_BAN_IP_WRONG_ID)
    {
        std::vector<char> ban_remaining_sec_data_buff(4);
        _receivePlain(this->getActiveSocket(), ban_remaining_sec_data_buff);

        //std::vector<char> cdata;
        emit sig_messageReceived(NULL, MSG_BAN_IP_WRONG_ID, ban_remaining_sec_data_buff);
    }// CMD_BAN_IP_WRONG_ID

    else if(command == CMD_ERROR_APP_VERSION_NOT_ACCEPTED)
    {
        std::vector<char> vurl;
        _receive(this->getActiveSocket(), vurl);

        emit sig_messageReceived(NULL, MSG_ERROR_APP_VERSION_NOT_ACCEPTED, vurl);
    } // CMD_ERROR_APP_VERSION_NOT_ACCEPTED

    else
    {
        qDebug ("---->> TO COMMAND DEN VRETHIKE : %s", &command[0]);
        return false;
    }

    return true;
}

//TODO: einai static afto?
//Detecting endianness programmatically in a C++ program
//http://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
void displayEndianness()
{
    if ( htonl(47) == 47 ) {
        qDebug("Big endian");
    } else {
        qDebug("Little endian");
    }
}

void clientserver::start_protocol()
{
        int bytes_recv;
        struct sockaddr_in serv_addr;
        struct hostent *SERVER;

        diffRequestCounter = 0;

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

        this->setActiveSocket(socket(AF_INET, SOCK_STREAM, 0));
#ifdef WIN32
        if (this->getActiveSocket() == INVALID_SOCKET) {
#else
        if (this->getActiveSocket() < 0){
#endif
            error("ERROR opening socket");
            return;
        }

        //SERVER = gethostbyname("mailgate.filoxeni.com");
        //SERVER = gethostbyname("andamaproxy-us-west.filoxeni.com");
        SERVER = gethostbyname("andamaproxy-ro-timisoara.filoxeni.com");
        //SERVER = gethostbyname("localhost");

        if (SERVER == NULL) {
            emit sig_messageReceived(NULL, MSG_NO_INTERNET_CONNECTION);
            fprintf(stderr, "ERROR, no such host\n");

#ifdef WIN32
            closesocket(this->getActiveSocket());
#else
            close(this->getActiveSocket());
#endif

            //kathistewrw ton termtismo tou thread
            //wste na min termatizei tin cpu
            std::chrono::milliseconds sleep_dura(1000);
            std::this_thread::sleep_for(sleep_dura);

            //edw xtypaei ean den exei syndesi sto internet o ypologistis
            return;
            //exit(0);
        }

        memset((char *) &serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;


        bcopy((char *)SERVER->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             SERVER->h_length);

        //serv_addr.sin_addr.s_addr=inet_addr("192.168.32.20"); // <------------- local server

        //SIMANTIKO: kanw disable to nagle algorithm. meiwnei to latency.
        int flag = 1;
        setsockopt(this->getActiveSocket(),      /* socket affected */
                                IPPROTO_TCP,     /* set option at TCP level */
                                TCP_NODELAY,     /* name of option */
                                (char *) &flag,  /* the cast is historical cruft */
                                sizeof(int));    /* length of option value */

            qDebug() << ">>>>>>>>>>>>>>  remoteport:" << remotePort;
            qDebug() << ">>>>>>>>>>>>>>  RemotePassword:" << QString(remotepassword.c_str());

            //to xrisimopoio gia domiki p2pclient
        if (remotePort != 0) {
            //p2p
            qDebug() << ">>>>>>>>>>>>> remoteIpAddress" << remoteIpAddress.c_str();
            serv_addr.sin_addr.s_addr=inet_addr(remoteIpAddress.c_str());
            //serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
            serv_addr.sin_port = htons(remotePort);
            //serv_addr.sin_port = htons(8085);
        }else{
            //sybdesi mesw proxy
            serv_addr.sin_port = htons(PROXY_PORT_NUMBER);
        }

        int conres = ::connect(this->getActiveSocket(), (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        if (conres < 0)
        {
            std::cout << "ERROR connecting. result: " << conres << "\n";

#ifdef WIN32
            closesocket(this->getActiveSocket());
#else
            close(this->getActiveSocket());
#endif
            if (remotePort == 0){
                emit sig_messageReceived(NULL, MSG_NO_PROXY_CONNECTION);
            }
            //edw xtypaei ean yparxei syndesi sto internet alla o proxy den trexei
            //error("ERROR connecting");

            return;
         }

        qDebug("Remote Cotrol BETA - (c) 2015 Yiannis Bourkelis \n\n");
        displayEndianness();

        //ksekinw tin epesksergasia twn commands
        std::array<char,1> cmdbuffer;

        //thetw socket options
        //int send_timeout = 30000;
        //int intlen = sizeof(int);
       // setsockopt(this->getActiveSocket(), SOL_SOCKET, SO_SNDTIMEO, (char*)&send_timeout, intlen);
        //getsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, &ilen);

        isClientConnected = false;

        while(true){
            try
            {
                //qDebug("1 -----> Waitting for command...");

                bytes_recv = recv(this->getActiveSocket(), &cmdbuffer[0], 1, 0);

                if (bytes_recv == 0){
                    std::cout << std::this_thread::get_id() << " " <<
                                 "######### --- Main command loop disconnected from server. ---- ########" << " " <<
                               "####  recv return 0 bytes. [MAIN command loop]. Returning from function." << std::endl;

        #ifdef WIN32
                    closesocket(this->getActiveSocket());
        #else
                    close(this->getActiveSocket());
        #endif
                    emit sig_messageReceived(NULL, MSG_NO_PROXY_CONNECTION);

                    return;
                }
                else if (bytes_recv == -1){
                    displayErrno("void clientserver::start_protocol() ## bytes_recv == -1 ## [MAIN command loop]. Returning from function.");

        #ifdef WIN32
                    closesocket(this->getActiveSocket());
        #else
                    close(this->getActiveSocket());
        #endif
                    emit sig_messageReceived(NULL, MSG_NO_PROXY_CONNECTION);

                    return;
                }

                //qDebug("2 -----> Command received: %s. Bytes: %i. Ksekina epeksergasia tou command",cmdbuffer.data(),bytes_recv);
                if (proccesCommand(cmdbuffer) == false){
#ifdef WIN32
                    closesocket(this->getActiveSocket());
#else
                    close(this->getActiveSocket());
#endif
                    return;
                }

            }
            catch(std::exception& ex)
            {
                qDebug("----> EXCEPTION sto start_protocol ::: %s", ex.what());
                isClientConnected = false;
                emit sig_exception(QString::fromUtf8(ex.what()));
            }

            catch( ... )
            {
                qDebug("----> EXCEPTION :: start_protocol unhandled exception");
                isClientConnected = false;
            }

        } // while


        #ifdef WIN32
        //closesocket(sockfd);
        #else
        //close(sockfd);
        #endif
        isClientConnected = false;

        return;
}
