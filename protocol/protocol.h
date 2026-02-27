#ifndef SENECA_PROTOCOL_H
#define SENECA_PROTOCOL_H
#include <string> 
#include "Credentials.h"
#include <optional>
namespace seneca{

    class Protocol{
        public: 
        virtual std::string createHandshake() const = 0;
        virtual std::string createAuthMessage(const Credentials& creds) const=0;
        virtual std::optional<std::string> processAuthResponse(const std::string& resp) const = 0;
        virtual ~Protocol() = default;


    };
}


#endif