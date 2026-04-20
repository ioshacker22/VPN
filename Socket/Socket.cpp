#include "Socket.h"
#include <iostream>
#include <stdexcept>

namespace seneca {

    Socket::Socket() : m_connected(false),m_address("127.0.0.1"),m_port(1194)   {}

    Socket::~Socket() {
        if (m_connected) {
            disconnect();
        }
    }

    void Socket::connect() {
        if (m_connected) {
            throw std::runtime_error("Socket: already connected.");
        }

        m_connected = true;
        std::cout << "[Socket] connected to " << m_address << ":" << m_port << "\n";
    }

    void Socket::sendData(const std::string& data) {
        if (!m_connected) {
            throw std::runtime_error("Socket: cannot send — not connected.");
        }
        std::cout << "[Socket] >> " << data << "\n";
    }

    std::string Socket::receiveData() {
        if (!m_connected) {
            throw std::runtime_error("Socket: cannot receive — not connected.");
        }
     
        static int callCount = 0;
        ++callCount;

        if (callCount % 2 == 1) {
            std::cout << "[Socket] << HELLO_ACK\n";
            return "HELLO_ACK";
        } else {
            const std::string response = "OK:SESSION-KEY-ABC123";
            std::cout << "[Socket] << " << response << "\n";
            return response;
        }
    }

    void Socket::disconnect() {
        if (!m_connected) {
            return;
        }
        // Real implementation would call ::close() / ::shutdown().
        m_connected = false;
        std::cout << "[Socket] disconnected from "
                  << m_address << ":" << m_port << "\n";
    }

    bool Socket::isConnected() const {
        return m_connected;
    }

} 