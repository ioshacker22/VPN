#include "VPNServer.h"

namespace seneca{

    VPNServer::VPNServer(): m_capacity(8), m_size(0), m_connections(nullptr), m_authDatabase(nullptr), m_authDatabaseSize(0){
        m_connections = new Connection*[m_capacity];
        for(size_t i = 0; i < m_capacity; i++){
            m_connections[i] = nullptr;
        }

        m_authDatabase = new Credential[m_capacity];
    
    }


    VPNServer::~VPNServer(){
        for(size_t i = 0; i < m_size; i++){
            delete m_connections[i];
        }

        delete[]m_authDatabase;
        delete[]m_connections;
    }

    VPNServer::VPNServer(const VPNServer& other) : m_capacity(other.m_capacity), m_size(other.m_size), m_connections(nullptr), m_authDatabase(nullptr), m_authDatabaseSize(other.m_authDatabaseSize){
        // Allocate connection array 
        m_connections = new Connection*[m_capacity];
        
        // Clone active connections
        for (size_t i = 0; i < m_size; i++) {
            m_connections[i] = other.m_connections[i]->clone();
        }
        
        // Initialize unused slots to nullptr 
        for (size_t i = m_size; i < m_capacity; i++) {
            m_connections[i] = nullptr;
        }
        
        // Allocate authentication database with capacity
        m_authDatabase = new Credential[m_capacity];
        
        // Copy credentials 
        for (size_t i = 0; i < m_authDatabaseSize; i++) {
            m_authDatabase[i] = other.m_authDatabase[i];
        }
    }

    VPNServer::VPNServer(VPNServer&& other) : m_connections (other.m_connections)
        ,m_authDatabase(other.m_authDatabase)
        ,m_capacity(other.m_capacity)
        ,m_size(other.m_size)
        ,m_authDatabaseSize(other.m_authDatabaseSize)
    {
        other.m_connections = nullptr;
        other.m_authDatabase = nullptr;
        other.m_capacity = 0;
        other.m_size = 0; 
        other.m_authDatabaseSize = 0;
    }


    VPNServer& VPNServer::operator=(const VPNServer& other){
        if(this != &other){

            //delete array of m_connections pointers
            for(size_t i = 0; i < m_size; i++){
                delete m_connections[i];
            }
            
            delete[]m_authDatabase;
            delete[] m_connections;

            //copy of capacity, size, authDatabaseSize from other
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_authDatabaseSize = other.m_authDatabaseSize;

            //allocating new m_connection array
            m_connections = new Connection*[m_capacity];
        
            // Clone active connections
            for (size_t i = 0; i < m_size; i++) {
                m_connections[i] = other.m_connections[i]->clone();
            }
            
            // Initialize unused slots to nullptr 
            for (size_t i = m_size; i < m_capacity; i++) {
                m_connections[i] = nullptr;
            }
            
            // Allocate authentication database with capacity
            m_authDatabase = new Credential[m_capacity];
            
            // Copy credentials 
            for (size_t i = 0; i < m_authDatabaseSize; i++) {
                m_authDatabase[i] = other.m_authDatabase[i];
            }
        }
        return *this;
    }

    VPNServer& VPNServer::operator=(VPNServer&& other){
        if(this != &other){
            
            //delete array of m_connection pointers
            for(size_t i = 0; i < m_size; i++){
                delete m_connections[i];
            }

            
            delete[]m_connections;
            delete[]m_authDatabase;

            //move attrivutes from other
            m_connections = other.m_connections;
            m_authDatabase = other.m_authDatabase;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_authDatabaseSize = other.m_authDatabaseSize;

            //reset other to emptystate
            other.m_connections = nullptr;
            other.m_authDatabase = nullptr;
            other.m_capacity = 0;
            other.m_size = 0; 
            other.m_authDatabaseSize = 0;

        }
        return *this;
    }

    
    void VPNServer::acceptConnection(Connection* c){
        if(c == nullptr){
            return;
        }
        
        // Resize if at capacity
        if(m_size == m_capacity){
            size_t newCapacity = m_capacity * 2;
            Connection** newArray = new Connection*[newCapacity];
            
            // Copy existing connections
            for(size_t i = 0; i < m_size; i++){
                newArray[i] = m_connections[i];
            }
            
            // Initialize remaining slots to nullptr
            for(size_t i = m_size; i < newCapacity; i++){
                newArray[i] = nullptr;
            }
            
            // Replace old array
            delete[] m_connections;
            m_connections = newArray;
            m_capacity = newCapacity;
        }
        
        // Add the incoming connection
        m_connections[m_size] = c;
        m_size++;
    }

    void VPNServer::removeConnection(const std::string& username){
        for(size_t i = 0; i < m_size; i++){

            //downcast to VPNClient to access getName()
            ConnectionTpl<AES>* client = dynamic_cast<ConnectionTpl<AES>*>(m_connections[i]);
        
            if(client != nullptr && client->getName() == username){
                delete m_connections[i];

                //shify connections after this one to the left
                for(size_t j = i; j < m_size - 1; j++){
                    m_connections[j] = m_connections[j+1];

                }
                m_connections[m_size -1] = nullptr;
                m_size--;
                return;  //stop afterremoving one connection
            }
        }
    }




}