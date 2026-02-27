#ifndef SENECA_PROTOCOL_H
#define SENECA_PROTOCOL_H
#include <string> 
namespace seneca{

    class Protocol{
        public: 
        virtual std::string createHandshake() const = 0;
        virtual std::string createAuthMessage(const std::string& message) const=0;
        virtual std::optional<std::string> processAuthResponse(const std::string& response) const = 0;
        virtual ~Protocol() = default;


    };
}


#endif