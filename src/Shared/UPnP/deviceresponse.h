#ifndef DEVICERESPONSE_H
#define DEVICERESPONSE_H

#include <string>
#include <QUrl>


class DeviceResponse
{
public:
    DeviceResponse();

    std::string rawResponse;
    QUrl descriptionUrl;
    std::string controlURL;
    std::string serviceName; //p.x. WANPPPConnection:1
};

#endif // DEVICERESPONSE_H
