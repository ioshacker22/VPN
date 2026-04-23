#include <string>
#include <iostream>
#include "VPNManager.h"
#include "VPNClient.h"
#include "AES.h"

namespace seneca{

    VPNManager::VPNManager(){
        std::cout << "[VPN] manager created" << std::endl;
    }

    VPNManager::~VPNManager(){
        std::cout << "[VPN] manager deleted" << std::endl;
    }

    void VPNManager::start(){
        std::cout << "[VPN] Server started" << std::endl;
    }

    void VPNManager::shutdown(){
        std::cout << "[VPN] Server shutdown" << std::endl;
    }

    void VPNManager::broadcast(const std::string& data){
        m_server.broadcastEncryptedData(data);
    }

    void VPNManager::addClient(const std::string& username, const std::string& password, VPNUser::AccessLevel level){
         // 1) Enforce policy
        if (!enforcePolicy(level)) {
            return; 
        }

        // 2) Create client 
        VPNClient<AES>* client = new VPNClient<AES>(username, password);

        // 3) Prepare client
        client->connect();
        client->handshake();
        client->authenticate();

        // 4) Transfer ownership to server
        m_server.acceptConnection(client);
    }

    void VPNManager::removeClient(const std::string& username){
        m_server.removeConnection(username);
    }

    bool VPNManager::enforcePolicy(VPNUser::AccessLevel level) const{
        return level >= VPNUser::AccessLevel::GUEST;
    }



}
