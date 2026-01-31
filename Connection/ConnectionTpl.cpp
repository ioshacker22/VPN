#include "ConnectionTpl.h"
#include "Connection.h"
#include <string>
#include <iostream> 

namespace seneca{

    template <typename T>
    ConnectionTpl<T>::ConnectionTpl() : m_socket(), m_cipher(), m_isAuthenticated(false), m_isConnected(false), m_sessionKey(){}

    template <typename T> 
    void ConnectionTpl<T>::connect(){
        //check state to see if connected
       if(m_isConnected){
        return;
       }

       //establish new connection
       m_socket.connect();

       //flip connection on
       m_isConnected = true;


    }

    template <typename T>
    void ConnectionTpl<T>::authenticate(){
        // secure connect to authenticate
        if (!m_isConnected) {
            return; 
        }
        
        // if Already authenticated 
        if (m_isAuthenticated) {
            return;  
        }
        
        // Send authentication credentials
        std::string authRequest = "AUTH_REQUEST";  
        m_socket.sendData(authRequest);
        
        //  Receive server response
        std::string authResponse = m_socket.receiveData();
        
        //  Validate server response
        if (authResponse.find("AUTH_OK: ") != 0) { 
            return;  
        }
        
        // Extract session key from response
        std::string sessionKey = authResponse.substr(8);
        
        if (sessionKey.empty()) {
            return; 
        }
        
        //  onfigure cipher with session key
        m_cipher.setKey(sessionKey);
        
        // Update state atomically (all three together)
        m_sessionKey = sessionKey;
        m_isAuthenticated = true;

    }

    template <typename T>
    void ConnectionTpl<T>::sendData(const std::string& plainText ){
        // must be authenticated
        if(!m_isConnected){
            return;
        }

        //Authenticate if not 
        if(!m_isAuthenticated){
            return;
        }

        //Encrypt data
        std::string encryptedData = m_cipher.encrypt(plainText);

        //send data through socket
        m_socket.sendData(encryptedData);

    }

    template <typename T>
    void ConnectionTpl<T>::receiveData(){
        //secure connection
        if(!m_isConnected){
            return;
        }

        //authenticate
        if(!m_isAuthenticated){
            return;
        }

        std::string encryptedData = m_socket.receiveData();
        std::string plaintext = m_cipher.decrypt(encryptedData);


        std::cout << plaintext << endl;
    }

    template <typename T>
    void ConnectionTpl<T>::disconnect(){
        //disconnect 
        if(!m_isConnected){
            return;
        }

        //close socket connection
        m_socket.disconnect();

        //clear session 
        m_sessionKey = "";
        m_isAuthenticated = false;
        m_isConnected = false;

    }



}