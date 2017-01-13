#ifndef IPPROTECTION_H
#define IPPROTECTION_H

//kathe fora pou ena client zita na syndethei se id
//pou den yparxei, ena instance aftis tis klasis kataxwreitai
//sto protect_id map.
//Me tin voitheia tis ginetai tracking ean to idio ip kanei id sniffing
class IPProtection
{
public:
    std::chrono::high_resolution_clock::time_point lastWrongIDTime; //i stigmi pou egine i teleftaia lathos apopeira
    int wrongIDCounter = 0;//to plithos twn lathos apopeirwn
};

#endif // IPPROTECTION_H
