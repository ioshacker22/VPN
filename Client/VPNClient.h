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
        Connection* clone()override;
        ~VPNClient()override = default;
        std::string getName() const override;
        Credentials buildCredentials() const override;


    };

    template <typename T>
    VPNClient<T>::VPNClient(const std::string& username, const std::string& password) : ConnectionTpl<T> (std::make_unique<BasicProtocol>()), m_username(username), m_password(password){}



    //clone copies configuration only
    template <typename T>
    Connection* VPNClient<T>::clone(){
        return new VPNClient<T>(this->m_username, this->m_password);
    }

    template <typename T>
    std::string VPNClient<T>::getName() const{
        return m_username;
    }
    //provvides credentials
   template <typename T>
    Credentials VPNClient<T>::buildCredentials() const {
        return Credentials{ m_username, m_password};
    }


}
#endif
