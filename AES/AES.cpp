#include "AES.h"
#include <stdexcept>

namespace seneca {

    void AES::setKey(const std::string& key) {
        if (key.empty()) {
            throw std::invalid_argument("AES: key must not be empty.");
        }
        m_key = key;
    }

    std::string AES::encrypt(const std::string& plaintext) const {
        if (m_key.empty()) {
            throw std::runtime_error("AES: cannot encrypt — key not set.");
        }

        std::string ciphertext(plaintext.size(), '\0');
        for (std::size_t i = 0; i < plaintext.size(); ++i) {
            ciphertext[i] = plaintext[i] ^ m_key[i % m_key.size()];
        }
        return ciphertext;
    }

 
    std::string AES::decrypt(const std::string& ciphertext) const {
        return encrypt(ciphertext);  
    }

} 