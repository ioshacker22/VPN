#include <iostream>
#include "../Client/VPNClient.h"
#include "../AES/AES.h"
#include "../Server/VPNServer.h"  

void test_double_disconnect(){
    //Arrange
    VPNClient<AES> client ("David", "passwords123");

    try{

    //act
    client.connect();
    client.disconnect();
    client.disconnect();

    //assert
    std::cout << "Pass: double disconnect did not crash\n";

    }catch(const std::exception& e){
        std::cout << "Exception thrown" << e.what() << "\n";
    }
}


//test wrong password 

void test_wrong_password(){
    VPNClient<AES> client ("John", "Bellstone59");

    try{
        //act 
        client.connect();
        client.handshake();
        client.authenticate();
        std::cout << "FAIL: authentication  should have thrown "  << std::endl;
    }catch(const std::logic_error& e){
         std::cout << "PASS: authentication rejected " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cout << "Exception thrown " << e.what() << std::endl;
    }
}



//test server capacity

void test_server_capacity(){
    VPNServer server; 

    //act - adding 9 clients 
    for(int i = 0; i < 9; i++){
        std::string m_username = "user" + std::to_string(i);
        std::string m_password = "pass" + std::to_string(i);

        auto* client = new VPNClient<AES>(m_username, m_password);

        client->connect();
        client->handshake();
        client->authenticate();

        //transfer ownership to server
        server.acceptConnection(client);
    }
    //assert
    server.broadcastEncryptedData("capacity test");
    std::cout << "test_server_capacity passed " << std::endl;
}


void test_send_before_auth(){
    VPNClient<AES> client("David", "password123");

    try{
        client.connect();

        client.sendData("secret message");

        std::cout << "FAIL: authentication should have thrown" << std::endl;
    }catch(const std::exception& e){
        std::cout << "PASS: correctly blocked sending before auth" << e.what() << std::endl;
    }
}


//edge cases

//empty username
void test_empty_username(){
    try{
        VPNClient<AES> client("", "Password123");

        std::cout << "FAIL: empty string did not pass through" << std::endl;
    }catch(const std::exception& e){
        std::cout << "PASS: empty username rejected" << e.what() << std::endl;
    }
}

//authenticate twice

void test_double_authenticate(){
    VPNClient<AES> client ("David", "password123");

    try{
        client.connect();
        client.handshake();
        client.authenticate();
        client.authenticate();

        std::cout << "FAIL: Already authenticated" << std::endl;
    }catch(const std::exception& e){
        std::cout << "Pass: cannot authenticate twice" <<e.what() << std::endl;
    }
}


//remove non-existent client

void test_remove_nonexistent(){
    VPNServer server; 

    try{
        server.removeConnection("NONEXISTENTUSER");
        std::cout << "PASS: removing non-existent client handled safely" << std::endl;
    }catch(const std::exception& e){
        std::cout << "FAIL: exception should not be thrown (" << e.what() << ")" << std::endl;
    }
}

//null connection 

void test_null_connection(){
    VPNServer server; 

    try{
        server.acceptConnection(nullptr);
        std::cout << "FAIL: nullptr should be rejected" << std::endl;
    }catch(const std::invalid_argument& e){
        std::cout << "PASS: nullptr rejected (" << e.what() << ")" << std::endl;
    }catch(const std::exception& e){
        std::cout << "FAIL: unexpected exception type" << e.what() << std::endl;
    }
}

//sendData empty string handling 

void test_empty_data(){
    VPNClient<AES> client("David", "password");

    try{
        client.connect();
        client.handshake();
        client.authenticate();
        client.sendData("");

        std::cout << "FAIL: sending empty data should not throw " << std::endl;

    }catch(const std::exception& e){
        std::cout << "PASS: empty data handled safely" << e.what()  << std::endl;
    }
}



int main(){

    test_double_disconnect();

    test_wrong_password();

    test_server_capacity();

    test_send_before_auth();

    test_empty_username();

    test_double_authenticate();

    test_remove_nonexistent();

    test_null_connection();

    test_empty_data();

    return 0;
}