#ifndef SENECA_VPNTUNNEL_H
#define SENECA_VPNTUNNEL_H

#include <iostream>
#include "Connection.h"
#include "Packet.h"

namespace seneca {
    class VPNTunnel{
        bool m_isOpen;
        Connection* m_connection;

        public:

        VPNTunnel(Connection* conn);

        //lifecycle
        void openTunnel();
        void closeTunnel();

        //dataflow
        void sendPacket(Packet& packet);
        Packet receivePacket() const;

        //state query
        bool isOpen() const;


    };


}
#endif