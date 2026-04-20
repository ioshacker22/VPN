#ifndef SENECA_PACKET_H
#define SENECA_PACKET_H

#include <string>
#include <cstddef>

namespace seneca {

    // Represents a single unit of data travelling through the VPN tunnel.
  
    class Packet {
        std::string m_data;
        std::size_t m_sequenceNumber;

    public:
        // Construct a packet with a payload and an optional sequence number
        explicit Packet(const std::string& data, std::size_t seqNum = 0);

        // Returns the raw payload string
        const std::string& getData() const;

        // Returns the sequence number 
        std::size_t getSequenceNumber() const;

        // Replace payload contents
        void setData(const std::string& data);
    };

} 

#endif