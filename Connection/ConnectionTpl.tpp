#include "ConnectionTpl.h"
#include "Connection.h"
#include <string>
#include <iostream> 


namespace seneca{



    //constructor: initializeds all members, starts in disconnected state
    template <typename T>
    ConnectionTpl<T>::ConnectionTpl(std::unique_ptr<Protocol>protocol) : m_socket(), m_cipher(), m_sessionKey(), m_protocol(std::move(protocol)), m_state(ConnectionState::Disconnected){}


    //return state name for error message and debugging
    template <typename T>
    std::string ConnectionTpl<T>::getStateName() const{
        switch (m_state){
        case ConnectionState::Disconnected:
            return "Disconnected";
        case ConnectionState::Connected:
            return "Connected";
        case ConnectionState::Handshaken:
            return "Handshaken";
        case ConnectionState::Authenticated:
            return "Authenticated";
        }
        return "Unknown";
    }

    template <typename T> 
    void ConnectionTpl<T>::connect(){
        //check state to see if connected
       if(m_state != ConnectionState::Disconnected){
          throw std::logic_error(getName() + ": already connected. Current state: " + getStateName());
        }

       //establish new connection
       m_socket.connect();

       //assign enum state
       m_state = ConnectionState::Connected;

    }



    template <typename T>
    void ConnectionTpl<T>::authenticate() {

        // Guard: must be connected first
        if (m_state != ConnectionState::Handshaken) {
            throw std::logic_error(getName() + ":  handshake must complete before authenticating." + getStateName() );
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
            throw std::runtime_error(getName() + ": authentication failed." + getStateName() );
        }

        // Cipher: configure with server-provided key
        m_cipher.setKey(*key);

        // Commit state atomically
        m_sessionKey = *key;
        m_state = ConnectionState::Authenticated;
    }

    template <typename T>
    void ConnectionTpl<T>::sendData(const std::string& plainText ){
        // must be authenticated
        if (m_state != ConnectionState::Authenticated) {
            throw std::logic_error(getName() + ": must be authenticated before sending data." + getStateName() );
        }

        //empty data check
        if(plainText.empty()){
            throw std::invalid_argument("cannot send empty data");
        }


        //Encrypt data
        std::string encryptedData = m_cipher.encrypt(plainText);

        //send data through socket
        m_socket.sendData(encryptedData);

    }

    template <typename T>
    std::string ConnectionTpl<T>::receiveData(){
        //secure connection
        if (m_state != ConnectionState::Authenticated) {
            throw std::logic_error(getName() + ": must be authenticated before receiving data." + getStateName() );
        }


        std::string encryptedData = m_socket.receiveData();
        std::string plaintext = m_cipher.decrypt(encryptedData);


        std::cout << plaintext << std::endl;

        return plaintext;
    }

    template <typename T>
    void ConnectionTpl<T>::disconnect(){

        //disconnect 
        if(m_state == ConnectionState::Disconnected){
            return;
        }

        //close socket connection
        m_socket.disconnect();

        //clear session 
        m_sessionKey.clear();
        m_cipher = T{};
        m_state = ConnectionState::Disconnected;
    

    }

    template <typename T> 
    void ConnectionTpl<T>::handshake() {

        //connecttion must be established before handshake can begin
        if(m_state != ConnectionState::Connected) {
            throw std::logic_error(getName() + ": must be connected before handshake." + getStateName() );
        }


        //builds opening message
        std::string hello = m_protocol->createHandshake();

        //send handshake opener to server
        m_socket.sendData(hello);

        //wait for server response
        std::string response = m_socket.receiveData();

        //validate server is ready to proceed
        if (response != "HELLO_ACK") {
            throw std::runtime_error("Handshake failed: invalid response");
        }

        // Handshake complete — authenticate() is now permitted
       m_state = ConnectionState::Handshaken;
    }







}