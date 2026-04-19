#include <iostream> 
#include "VPNUser.h"

namespace seneca {

    VPNUser::VPNUser() : m_username(), m_access(AccessLevel::USER), m_state(SessionState::LOGGED_OUT){}

    bool VPNUser::login(){
        if(m_state != SessionState::LOGGED_IN){
            m_state =  SessionState::LOGGED_IN;
            return true;
        }
        return false;
    }

    void VPNUser::logout(){
        if(m_state != SessionState::LOGGED_OUT){
            m_state = SessionState::LOGGED_OUT;
        }
    }

    bool VPNUser::hasPermission(AccessLevel required) const{
       return m_access >= required;
    }




}