#include "deviceresponse.h"

DeviceResponse::DeviceResponse()
{

}

DeviceResponse::DeviceResponse(const DeviceResponse &obj)
{
   controlURL = obj.controlURL;
   descriptionUrl = obj.descriptionUrl;
   rawResponse = obj.rawResponse;
   serviceName = obj.serviceName;
}
