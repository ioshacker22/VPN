#include <iostream>
#include "VPNServer.h"
#include "VPNClient.h"
#include "AES.h"
#include "BasicProtocol.h"
#include "VPNTunnel.h"
#include "Packet.h"
using namespace seneca;

int main(){

 // create vpn server
    VPNServer server;
    std::cout << "[VPN] Server started" << std::endl;

    // create vpn client
    VPNClient<AES>* client = new VPNClient<AES>("David", "password123");

    // attempt connection
    std::cout << "[VPN] Client attempting connection..." << std::endl;
    client->connect();

    // handshake
    client->handshake();

    // authenticate
    client->authenticate();
    std::cout << "[AUTH] User authenticated" << std::endl;

    // accept client on server
    server.acceptConnection(client);

    // create tunnel using client's connection
    VPNTunnel tunnel(client);   // assumes tunnel takes Connection*

    // open tunnel
    tunnel.openTunnel();
    std::cout << "[TUNNEL] Secure tunnel established" << std::endl;

    // send data through tunnel
    Packet packet("Hello secured world");
    tunnel.sendPacket(packet);
    std::cout << "[DATA] Encrypted packet sent" << std::endl;

    // receive data
    tunnel.receivePacket();
    std::cout << "[DATA] Encrypted packet received" << std::endl;

    // close tunnel
    tunnel.closeTunnel();
    std::cout << "[TUNNEL] Tunnel closed" << std::endl;

    // remove client
    server.removeConnection("David");
    std::cout << "[VPN] Client disconnected" << std::endl;




    return 0;
}