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
};

#endif // DEVICERESPONSE_H
