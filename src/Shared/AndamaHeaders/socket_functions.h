#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

#include <vector>
#include <array>
#include <mutex>
#include <iostream>
#include <thread>

#include "../Shared/AndamaHeaders/byte_functions.h"
#include "../Shared/AndamaHeaders/exception_helper.h"

#define NS_INADDRSZ  4
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ   2

//apostelei to message
#ifdef WIN32
int _sendmsgPlain(const SOCKET socketfd, const std::vector<char> &message);
#else
int _sendmsgPlain(const int socketfd, const std::vector<char> &message);
#endif

//apostelei to command(1byte) kai to mynima efoson yparxei
#ifdef WIN32
int _sendmsgPlain(const SOCKET socketfd, const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
#else
int _sendmsgPlain(const int socketfd, const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
#endif


/* apostelei:
 * - to command (1 byte)
 * - ypologizei me 4 byte to megethos tou mynimatos
 * - to mynima
 * - Veltiwseis pou tha mporousan na ginoun:
 * a)anti na antigrafw to message sto msg, na kataxwrw to msg stin arxi tou message
 */
#ifdef WIN32
int _sendmsg(const SOCKET socketfd, const std::array<char, 1> &command, const std::vector<char> &message);
#else
int _sendmsg(const int socketfd,    const std::array<char, 1> &command, const std::vector<char> &message);
#endif

//lamvanei bytes isa me to length tou charbuffer pou pernaei byref
#ifdef WIN32
int _receivePlain(const SOCKET socketfd, std::vector<char> &charbuffer);
#else
int _receivePlain(const int socketfd, std::vector<char> &charbuffer);
#endif


/* kanei de-serialize ta prwta 4 bytes tou minimatos
 * pou perigrafoun to megethos tou mynimatos pou akolouthei
 * kai sti synexei lamvanoun to mynima.
 * Afto to mynima to fortwnei sto charbuffer byref
 */
#ifdef WIN32
int _receive(const SOCKET socketfd, std::vector<char> &charbuffer);
#else
int _receive(const int socketfd, std::vector<char> &charbuffer);
#endif

//syndeetai stin ip kai port eisodou kai epistrefei to socket
int getClientSocket(std::string IP, unsigned short int Port);

//anti gia tin inet_pton pou den yparxei sto mingw compiler
//https://stackoverflow.com/questions/15370033/how-to-use-inet-pton-with-the-mingw-compiler
int inet_pton_mingw(int af, const char *src, char *dst);
int inet_pton4_mingw(const char *src, char *dst);
int inet_pton6_mingw(const char *src, char *dst);

#endif // SOCKET_FUNCTIONS_H
