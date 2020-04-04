#ifndef FINALLY_H
#define FINALLY_H

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
