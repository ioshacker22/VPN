#ifndef SENECA_BASICPROTOCOL_H
#define SENECA_BASICPROTOCOL_H

#include <string> 
#include <optional>
#include "protocol.h"

namespace seneca{

    class BasicProtocol : public  Protocol{
        public:
       
        std::string createHandshake() const override;
        virtual std::string createAuthMessage(const std::string& message) const override;

        virtual std::optional<std::string> processAuthResponse(const std::string& response) const override;

        virtual ~BasicProtocol()=default;

    };



}
#endif