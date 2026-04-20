#include "Packet.h"

namespace seneca {

    Packet::Packet(const std::string& data, std::size_t seqNum)
        : m_data(data), m_sequenceNumber(seqNum)
    {}

    const std::string& Packet::getData() const {
        return m_data;
    }

    std::size_t Packet::getSequenceNumber() const {
        return m_sequenceNumber;
    }

    void Packet::setData(const std::string& data) {
        m_data = data;
    }

} // namespace seneca