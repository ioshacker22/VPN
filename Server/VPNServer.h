#ifndef SENECA_VPNSERVER_H
#define SENECA_VPNSERVER_H
#include <string>
#include <cstddef>
#include "Connection.h"


namespace seneca{

    class VPNServer{
        size_t m_capacity;
        size_t m_size;
        Connection** m_connections;

        struct Credential {
        std::string username;
        std::string password;
        };
        
        Credential* m_authDatabase; //dynamic array of valid credentials
        size_t m_authDatabaseSize; // Number of valid credentials

        public:
        VPNServer();

        //Rule of 5
        VPNServer(const VPNServer& other);
        VPNServer& operator=(const VPNServer& other);
        ~VPNServer();
        VPNServer(VPNServer&& other);
        VPNServer& operator=(VPNServer&& other);

        //member functions
        void acceptConnection(Connection* c);
        void removeConnection(const std::string& username);
        void broadcastEncryptedData(const std::string& data);

    };
   




}
#endif