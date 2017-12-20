#ifndef CLIENTINFO_H
#define CLIENTINFO_H

//gia kathei client pou syndeetai ston server
//kataxaritai sto clients map ena instance tis klasis me plirofories gia ton client
class ClientInfo
{
public:
    int sockfd = -1; //to socket epikoinwnias me ton client
    int remote_socket = -1; //to socket epikoinwnias me ton remote client an ginei accept
    std::string remote_id = ""; //to id tou remote client ean exei ginei accept
    std::vector<char> cachedID; //to cachedID pou xrisiopoieitai gia na diatirei o client to idio ID se periptwsi aposyndesis
    unsigned short port = 0; //i port pou epitrepei p2p connection (ean exei anoixtei me uPnP i manually)

    //i ip tou client
#ifdef WIN32
    unsigned long ip = 0;
#else
    in_addr_t ip = 0;
#endif

};

#endif // CLIENTINFO_H
