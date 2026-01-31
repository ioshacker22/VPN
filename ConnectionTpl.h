#include <string> 
#include "Connection.h"
#include "Socket.h"

namespace seneca{


template <typename T>
class ConnectionTpl : public Connection{

    Socket m_socket;
    T m_cipher;
    bool m_isAuthenticated; 
    std::string m_sessionKey;

    public: 
    // Default constructor: creates disconnected, unauthenticated connection
    // State after construction:
    // Socket exists but not connected
    // Cipher initialized but not keyed
    //  m_isAuthenticated = false
    //  m_sessionKey = empty
    // Network actions happen later via connect()
    ConnectionTpl();
    void connect() override;
    void authenticate()override;
    void sendData(const std::string& )override;
    void receiveData() override;
    void disconnect()override;
    ~ConnectionTpl()override;
    Connection* clone()override ;
};

}
