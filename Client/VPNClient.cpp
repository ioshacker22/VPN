#include <string> 
#include "VPNClient.h"
#include "ConnectionTpl.h"

namespace seneca{

    template <typename T>
    VPNClient<T>::VPNClient(const std::string& username, const std::string& password) : ConnectionTpl<T> (), m_username(username), m_password(password){}

    template <typename T>
    void VPNClient<T>::authenticate(){
        //secure connection
        if(!m_isConnected){
            return;
        }

        //authenticate
        if(m_isAuthenticated){
            return;
        }

        //send credentials
        std::string credentials = m_username + ":" + m_password;
       m_socket.sendData(credentials);

       //delegate authentication
       ConnectionTpl<T>::authenticate();
    }

    template <typename T>
    Connection* VPNClient<T>::clone(){
        return new VPNClient<T>(m_username, m_password);
    }



}