#include <iostream>
#include <stdexcept>
#include "VPNTunnel.h"

namespace seneca{

    VPNTunnel::VPNTunnel(Connection* conn): m_isOpen(false), m_connection(conn){}

    void VPNTunnel::openTunnel(){
        if(m_isOpen){
            return;
        }else{
            m_isOpen = true;  
        }
    }

    void VPNTunnel::closeTunnel(){
        if(m_isOpen){
            m_isOpen = false;
        }
    }

    void VPNTunnel::sendPacket(Packet& packet){
        if(!m_isOpen){
            return;
        }

        if(m_connection == nullptr){
            throw std::runtime_error("No connections");
        }

        m_connection->sendData(packet.getData());
    }

    //returns state
    bool VPNTunnel::isOpen()const {
        return m_isOpen;
    }


}