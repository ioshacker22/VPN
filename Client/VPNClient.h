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
        Connection* clone()override;
        ~VPNClient()override = default;
        std::string getName() const override;


    };

    template <typename T>
    VPNClient<T>::VPNClient(const std::string& username, const std::string& password) : ConnectionTpl<T> (), m_username(username), m_password(password){}

    template <typename T>
    void VPNClient<T>::authenticate(){
        //secure connection
        if(!this->m_isConnected){
            return;
        }

        //authenticate
        if(this->m_isAuthenticated){
            return;
        }

        //send credentials
        std::string credentials = m_username + ":" + m_password;
       this->m_socket.sendData(credentials);

       //delegate authentication
       ConnectionTpl<T>::authenticate();
    }

    //clone copies configuration only
    template <typename T>
    Connection* VPNClient<T>::clone(){
        return new VPNClient<T>(this->m_username, this->m_password);
    }

    
    template<typename T> 
    std::string VPNClinet<T>::buildCredentials()const{
        return m_username + ":" + m_password;
    }



}
#endif
