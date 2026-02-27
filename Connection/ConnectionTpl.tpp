#include "ConnectionTpl.h"
#include "Connection.h"
#include <string>
#include <iostream> 


namespace seneca{

    template <typename T>
    ConnectionTpl<T>::ConnectionTpl(std::unique_ptr<Protocol>protocol) : m_socket(), m_cipher(), m_isAuthenticated(false), m_isConnected(false), m_sessionKey(), m_protocol(std::move(protocol)){}

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
    void ConnectionTpl<T>::authenticate() {

        // Guard: must be connected first
        if (!m_isConnected) {
            throw std::logic_error(getName() + ": must be connected before authenticating.");
        }

        // Guard: don't re-authenticate
        if (m_isAuthenticated) {
            throw std::logic_error(getName() + ": already authenticated.");
        }

        // Identity: ask derived class for credentials
        Credentials creds = buildCredentials();

        // Protocol: serialize credentials into wire format
        std::string authMessage = m_protocol->createAuthMessage(creds);

        // Transport: send and receive
        m_socket.sendData(authMessage);
        std::string response = m_socket.receiveData();

        // Protocol: validate response and extract session key
        auto key = m_protocol->processAuthResponse(response);

        if (!key) {
            throw std::runtime_error(getName() + ": authentication failed or malformed response.");
        }

        // Cipher: configure with server-provided key
        m_cipher.setKey(*key);

        // Commit state atomically
        m_sessionKey      = *key;
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


        std::cout << plaintext << std::endl;
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