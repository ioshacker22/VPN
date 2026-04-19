#ifndef SENECA_VPNMANAGER_H
#define SENECA_VPNMANAGER_H

#include <string>
#include "VPNServer.h"
#include "VPNUser.h"

namespace seneca{

    class VPNManager{
        VPNServer m_server;
        bool enforcePolicy(VPNUser::AccessLevel level) const;

        public: 

        

        //lifecycle
        VPNManager();
        ~VPNManager();

        // start system
        void start();

        // create, prepare, and add a client 
        void addClient(const std::string& username, const std::string& password, VPNUser::AccessLevel level = VPNUser::AccessLevel::USER);

        // broadcast through server
        void broadcast(const std::string& data);

        // clean shutdown
        void shutdown();

        //remove client
        void removeClient(const std::string& username);

    };


}
#endif