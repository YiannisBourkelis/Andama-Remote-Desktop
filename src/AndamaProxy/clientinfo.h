#ifndef CLIENTINFO_H
#define CLIENTINFO_H

//gia kathei client pou syndeetai ston server
//kataxaritai sto clients map ena instance tis klasis me plirofories gia ton client
class ClientInfo
{
public:
    int sockfd = -1; //to socket epikoinwnias me ton client
    int remote_socket = -1; //to socket epikoinwnias me ton remote client an ginei accept
    std::string remote_id = ""; //to id to remote client ean exei ginei accept
    std::vector<char> cachedID; //to cachedID pou xrisiopoieitai gia na diatirei o client to idio ID se periptwsi aposyndesis
};

#endif // CLIENTINFO_H
