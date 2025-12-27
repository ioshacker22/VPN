#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
using namespace std; 

int main(){

    //create a new socket and return

    int serverSocket= socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0){
            cout<< "socket creation failed";
            return 1;
        }else{
            cout << "Server starting..." << endl;
        }
    
        


    //store socket address + port 
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    

    
    // Bind socket to address
    int bind_Result = ::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bind_Result < 0) {
        cout <<"Bind failed";
        return 1;
    }


    
   // mark socket as listening
    int listenResult = listen(serverSocket, 5);
    if(listenResult < 0){
        cout<< "listen failed";
        return 1;
    }else{
        cout << "Waiting for connection on port 8080..." << endl;  
    }

  
    //accept connection 
    //accept incoming connection and return a new socket descriptor
    int clientSocket = accept(serverSocket, NULL, NULL);
    if(clientSocket < 0){
        cout<< "Failed acceptance";
        return 1;
    }else{
        cout << "Client connected!" << endl;
    }

    // receive data   call recv()
    char buffer[1024];
    ssize_t receive = recv(clientSocket, buffer, sizeof(buffer), 0);
 
    if(receive < 0){
        cout<< "Failed receive";
       return 1;
    }else{
        cout << "Received: " << buffer << endl;
    }

    //send data call send()
    //send data to connected socket//
    ssize_t BytesSent = send(clientSocket, buffer, receive, 0);
    if(BytesSent < 0){
        cout<< "failed send";
        return 1;
    }else{
        cout << "Server closing..." << endl;
    }

       
    // close socket call close()

    close(clientSocket);
    close(serverSocket);

    return 0;
}