#include <string> 
#include "Connection.h"
#include "Socket.h"
#include <memory>
#include "Protocol.h"
#include "Credentials.h"


namespace seneca{


template <typename T>
class ConnectionTpl : public Connection{
    protected:
    Socket m_socket;
    T m_cipher;
    bool m_isAuthenticated; 
    bool m_isConnected;
    std::string m_sessionKey;
    std::unique_ptr<Protocol> m_protocol;

    protected: 
    virtual Credentials buildCredentials() const = 0;
   
    

    public: 
    // Default constructor: creates disconnected, unauthenticated connection
    // State after construction:
    // Socket exists but not connected
    // Cipher initialized but not keyed
    //  m_isAuthenticated = false
    //  m_sessionKey = empty
    // m_protocol = move properties 
    // Network actions happen later via connect()
    ConnectionTpl(std::unique_ptr<Protocol>protocol);
    void connect() override;
    void authenticate()override;
    void sendData(const std::string& )override;
    void receiveData() override;
    void disconnect()override;


};

}
