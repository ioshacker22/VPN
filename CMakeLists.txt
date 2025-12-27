cmake_minimum_required(VERSION 3.16) 
project(VPN VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)


add_executable(VPN
    SRC/simple_server.cpp
)


target_link_libraries(VPN
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto
) 