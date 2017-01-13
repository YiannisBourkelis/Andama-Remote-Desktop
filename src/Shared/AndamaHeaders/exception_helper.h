#ifndef EXCEPTION_HELPER_H
#define EXCEPTION_HELPER_H

#include <string>
#include <iostream>
#include <thread>

#ifdef WIN32
#define NOMINMAX
#include <stdio.h>
#else
#include "sys/time.h"
#endif

//emfanizei to teleftaio sfalma pou proklithike
void displayErrno(std::string source);

std::string getTime();

#endif // EXCEPTION_HELPER_H
