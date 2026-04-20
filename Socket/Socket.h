#ifndef SENECA_SOCKET_H
#define SENECA_SOCKET_H

#include <string>

namespace seneca{

    class Socket
    {
    private:
        bool m_connected;
        std::string m_address;
        int m_port;

    public:
        Socket();

        // Prevent copy 
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        ~Socket();
       
        // Establish connection to a default/configured endpoint
        void connect();

        // Send a string payload
        void sendData(const std::string& data);

        // Block until data arrives and return it
        std::string receiveData();

        // Tear down the connection
        void disconnect();

        bool isConnected() const;
    };

}

#endif