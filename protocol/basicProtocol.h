#ifndef SENECA_BASICPROTOCOL_H
#define SENECA_BASICPROTOCOL_H

#include <string> 
#include "protocol.h"

namespace seneca{

    class BasicProtocol : public  Protocol{
        public:
       
        std::string createHandshake() const override;
        virtual std::string createAuthMessage(const std::string& message) const override;
        virtual bool validateAuthMessage(const std::string& message) const override;
        virtual std::string createSessionKey() const override;
        virtual ~BasicProtocol()=default;

    };



}
#endif