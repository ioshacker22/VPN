#include <iostream>
#include "VPNServer.h"
#include "VPNClient.h"
#include "AES.h"
#include "BasicProtocol.h"

int main(){

    //create vpn server
    VPNServer server;
    std::cout << "VPN server created" <<std::endl;

    //create vpn client
    VPNClient<AES>* client = new VPNClient<AES>(
        "David",
        "password123"
    );

    //attempt connection
    client->connect();

    //Attempt handshake 
    client->handshake();

    //authenticate client 
    client->authenticate();

    //accept client on server side
    server.acceptConnection(client);

    //broadcast  data 
    server.broadcastEncryptedData("Hello secured world");

    //remove client
    server.removeConnection("David");



    return 0;
}