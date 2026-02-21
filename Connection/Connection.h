#ifndef SENECA_CONNECTION_H
#define SENECA_CONNECTION_H
#include <string>

namespace seneca{


class Connection{

    public: 
    virtual void connect() =0;
    virtual void authenticate() = 0;
    virtual void sendData(const std::string& ) = 0;
    virtual void receiveData() = 0;
    virtual void disconnect() = 0;
    virtual ~Connection() = 0;
    virtual Connection* clone() = 0;
    virtual std::string getName()const = 0;

};
Connection::~Connection(){};

}

#endif