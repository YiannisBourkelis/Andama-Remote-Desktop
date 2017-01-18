#ifndef ADDPORTMAPPINGRESPONSE_H
#define ADDPORTMAPPINGRESPONSE_H

#include <string>

class AddPortMappingResponse
{
public:
    AddPortMappingResponse();

    unsigned short statusCode;
    unsigned short remotePort;
    unsigned short internalPort;
    std::string portMappingDescription;
    std::string rawResponse;
};

#endif // ADDPORTMAPPINGRESPONSE_H
