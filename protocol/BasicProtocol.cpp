#include "BasicProtocol.h"

namespace seneca{

  //starts protocol conversation. 
  std::string BasicProtocol::createHandshake() const{
    return "Hello";
  }  

  //ormats outgoing auth payload
  std::string BasicProtocol::createAuthMessage(const std::string& message) const{
    return "AUTH:" + message;
  }

 
  //Takes servers response string and either extracts a session key or reports failure
  std::optional<std::string> BasicProtocol::processAuthResponse(const std::string& response) const{
    const std::string prefix = "OK:";

    if(response.rfind(prefix, 0) != 0){
      return std::nullopt;
    }

     // Extract everything after "OK:"
    std::string sessionKey = response.substr(prefix.size());

    if (sessionKey.empty()) {
        return std::nullopt;   
    }

    return sessionKey; 

  }


}