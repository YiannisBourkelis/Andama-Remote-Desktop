#ifndef EXCEPTION_HELPER_H
#define EXCEPTION_HELPER_H

#include <string>
#include <iostream>
#include <thread>
#include <cstring> //gia to errno kai strerr se linux

#ifdef WIN32
#include <ws2tcpip.h>
#else
#include "sys/time.h"
#endif

//emfanizei to teleftaio sfalma pou proklithike
void displayErrno(std::string source);

std::string getTime();

#endif // EXCEPTION_HELPER_H
