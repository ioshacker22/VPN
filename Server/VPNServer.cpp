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






}