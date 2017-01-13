#include "exception_helper.h"

//emfanizei to teleftaio sfalma pou proklithike
void displayErrno(std::string source)
{
#ifdef WIN32
    std::cout << getTime() << " " << std::this_thread::get_id() << " " <<
                 "###### [source: " << source <<  "]  displayErrno: " << strerror(WSAGetLastError()) << std::endl;
#else
    std::cout << getTime() << " " << std::this_thread::get_id() << " " <<
                 "###### [source: " << source <<  "]  displayErrno: " << strerror(errno) << std::endl;
#endif
}


//console logging
std::string getTime()
{
#ifdef WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    std::string ret;
    ret.insert(ret.begin(),st.wYear);
    return ret;
#else
    struct timeval tv;
    time_t nowtime;
    struct tm *nowtm;
    char tmbuf[64], buf[64];

    gettimeofday(&tv, NULL);
    nowtime = tv.tv_sec;
    nowtm = localtime(&nowtime);
    strftime(tmbuf, sizeof tmbuf, "[%Y-%m-%d %H:%M:%S", nowtm);
    snprintf(buf, sizeof buf, "%s.%06lu]", tmbuf, tv.tv_usec);
    return std::string(buf);
#endif
}
