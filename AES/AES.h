#ifndef SENECA_AES_H
#define SENECA_AES_H

#include <string>

namespace seneca {

    // Simulated AES cipher.

    class AES {
        std::string m_key;

    public:
        AES() = default;
        AES(const AES&) = default;
        AES& operator=(const AES&) = default;
        ~AES() = default;

        // Store the session key provided by the server after authentication
        void setKey(const std::string& key);

        // Encrypt plaintext - returns encoded ciphertext string
        std::string encrypt(const std::string& plaintext) const;

        // Decrypt ciphertext - returns recovered plaintext string
        std::string decrypt(const std::string& ciphertext) const;
    };

}

#endif