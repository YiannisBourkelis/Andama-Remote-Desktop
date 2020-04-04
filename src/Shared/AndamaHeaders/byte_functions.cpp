#include "byte_functions.h"
#include <stdexcept>

//metatropi (serialization) enos int se vector<char> wste na ginetai
//dynati i metafora tou sto socket.
//to buffer pernaei byref kai analoga me tin arxikopoiisi pou tou exei ginei
//diladei ean exei length 1,2 h 4 bytes ginetai i analogi metatropi
void intToBytes(const int value, std::vector<char> &buffer)
{
    if(buffer.size() == 4){
        buffer[0] = (value >> 24) & 0xff;
        buffer[1] = (value >> 16) & 0xff;
        buffer[2] = (value >> 8)  & 0xff;
        buffer[3] = value & 0xff;
    }
    else if(buffer.size() == 2){
        buffer[0] = (value >> 8)  & 0xff;
        buffer[1] = value & 0xff;
    }
    else if(buffer.size() == 1){
        buffer[0] = value & 0xff;
    }
}

//lamvanei ena vector<char> pou periexei arithmo pou egine
//serialize me to intToBytes kai epistrefei ton arxiko arithmo.
//epitrepetai na exei megethos 1,2 h 4 bytes.
int bytesToInt(const std::vector<char> &buffer)
{
    if (buffer.size() == 4){
        return
            (((unsigned char)buffer[0]) << 24) +
            (((unsigned char)buffer[1]) << 16) +
            (((unsigned char)buffer[2]) << 8) +
              (unsigned char)buffer[3];
    }
    if (buffer.size() == 2){
        return
            (((unsigned char)buffer[0]) << 8) +
              (unsigned char)buffer[1];
    }
    else if (buffer.size() == 1){
        return
              (unsigned char)buffer[0];
    }
    return 0;
}


//TODO: giati edw to ekana unsigned int? Den tha eprepe na einai int?
unsigned int bytesToInt(const std::vector<char> &buffer, unsigned int begin, unsigned int bytes)
{
    std::vector<char> ret(buffer.begin() + begin,buffer.begin() + begin + bytes);
    return bytesToInt(ret);
}

void ulongToBytes(unsigned long value, std::vector<char> &buffer)
{
    if(buffer.size() == 4){
        buffer[0] = (value >> 24) & 0xff;
        buffer[1] = (value >> 16) & 0xff;
        buffer[2] = (value >> 8)  & 0xff;
        buffer[3] = value & 0xff;
    } else {
        throw std::runtime_error("void ulongToBytes: buffer size out of range");
    }
}

unsigned long bytesToULong(const std::vector<char> &buffer)
{
    if (buffer.size() == 4){
        return
                (((unsigned char)buffer[0]) << 24) +
                (((unsigned char)buffer[1]) << 16) +
                (((unsigned char)buffer[2]) << 8) +
                  (unsigned char)buffer[3];
    }
    throw std::runtime_error("unsigned long bytesToULong: buffer size out of range");
}

/*
void ulongToBytes(unsigned long value, std::vector<char> &buffer)
{
    if(buffer.size() == 4){
        buffer[0] = (int)((value >> 24) & 0xFF) ;
        buffer[1] = (int)((value >> 16) & 0xFF) ;
        buffer[2] = (int)((value >> 8) & 0XFF);
        buffer[3] = (int)((value & 0XFF));
    } else {
        throw std::runtime_error("void ulongToBytes: buffer size out of range");
    }
}

unsigned long bytesToULong(const std::vector<char> &buffer)
{
    if (buffer.size() == 4){
        return
                ( (buffer[0] << 24)
                + (buffer[1] << 16)
                + (buffer[2] << 8)
                + (buffer[3] ) );
    }
    throw std::runtime_error("unsigned long bytesToULong: buffer size out of range");
}
*/

unsigned long bytesToULong(const std::vector<char> &buffer, unsigned int begin, unsigned int bytes)
{
    std::vector<char> ret(buffer.begin() + begin,buffer.begin() + begin + bytes);
    return bytesToULong(ret);
}
