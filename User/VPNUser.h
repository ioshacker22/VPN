#ifndef SENECA_VPNUSER_H
#define SENECA_VPNUSER_H


#include <string>

namespace seneca {

    class VPNUser{

     
        enum class SessionState{
            LOGGED_IN,
            LOGGED_OUT,
        };

        public:
        enum class AccessLevel {
            GUEST,
            USER,
            ADMIN
        };

        private:
        std::string m_username;
        AccessLevel m_access;
        SessionState m_state;


        public: 

        VPNUser();
        bool login();
        void logout();
        bool hasPermission(AccessLevel required) const;


    };

}
#endif