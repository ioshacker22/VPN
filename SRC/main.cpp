#include <iostream> 
#include <openssl/ssl.h>
#include <boost/asio.hpp>
#include <openssl/crypto.h>

using namespace std;

int main(){

    cout<<"Environment setup successful"<<'\n';
    cout << OpenSSL_version(OPENSSL_VERSION) << '\n';

    return 0;
}