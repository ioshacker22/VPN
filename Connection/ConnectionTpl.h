#ifndef SENECA_CONNECTIONTPL_H
#define SENECA_CONNECTIONTPL_H

#include <string> 
#include "Connection.h"
#include "Socket.h"
#include <memory>
#include "protocol.h"
#include "Credentials.h"


namespace seneca{

    //state management responsibility
    enum class ConnectionState{
        Disconnected,
        Connected,
        Handshaken,
        Authenticated
    };


    template <typename T>
    class ConnectionTpl : public Connection{
        private:
        Socket m_socket;
        T m_cipher;
        std::string m_sessionKey;
        std::unique_ptr<Protocol> m_protocol;
        ConnectionState m_state;

        protected:
        virtual Credentials buildCredentials() const = 0;
        std::string getStateName() const;
       

    
        

        public: 
        // Default constructor: creates disconnected, unauthenticated connection
        // State after construction:
        // Socket exists but not connected
        // Cipher initialized but not keyed
        //  m_sessionKey = empty
        // m_protocol = move properties 
        // Network actions happen later via connect()
        ConnectionTpl(std::unique_ptr<Protocol>protocol);
        void connect() override;
        void authenticate()override;
        void sendData(const std::string& )override;
        std::string receiveData() override;
        void disconnect()override;
        void handshake() override;


};

}

#include "ConnectionTpl.tpp"
#endif