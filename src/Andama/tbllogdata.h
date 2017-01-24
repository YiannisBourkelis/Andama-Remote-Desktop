#ifndef TBLLOGDATA_H
#define TBLLOGDATA_H

#include <chrono>
#include <string>

class TblLogData
{
public:
    TblLogData();
    TblLogData(std::string eventDesc);

    //std::chrono::time_point<std::chrono::system_clock> dateTime;
    time_t dateTime; // = time(0);
    std::string eventDescription;
    std::string getDateTimeString() const;

public:
};

#endif // TBLLOGDATA_H
