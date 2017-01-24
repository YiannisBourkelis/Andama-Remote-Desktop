#include "tbllogdata.h"
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string

TblLogData::TblLogData()
{
    dateTime = std::chrono::system_clock::now();
}

TblLogData::TblLogData(std::string eventDesc) : TblLogData::TblLogData()
{
    eventDescription = eventDesc;
}

std::string TblLogData::getDateTimeString() const
{
    auto in_time_t = std::chrono::system_clock::to_time_t(dateTime);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
