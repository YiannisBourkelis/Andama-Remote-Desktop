#ifndef FINALLY_H
#define FINALLY_H

//ektelei ena tmima codika prin vgei out of scope
//xrisi: Finally finally([&]{addPortMappingPendingRequests--;});
//opou p.x. to addPortMappingPendingRequests einai ena counter
class Finally
{
public:
    Finally (std::function<void ()> func) : m_func (func)
    {
    }

    ~Finally ()
     {
        m_func ();
     }

private:
    std::function<void ()> m_func;
};

#endif // FINALLY_H
