#ifndef SENECA_VPNCLIENT_H
#define SENECA_VPNCLIENT_H
#include <string> 
#include "ConnectionTpl.h"

namespace seneca{

    template <typename T>
    class VPNClient : public ConnectionTpl<T>{
        std::string m_username;
        std::string m_password;

        public:
        //constructor accepts username and password//
        VPNClient(const std::string& username, const std::string& password);

        //override methods
        void authenticate()override;
        seneca::Connection* clone()override;
        ~VPNClient()override;


    };



}
#endif
