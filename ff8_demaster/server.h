#pragma once
#include <asio.hpp>
#include <string>

class server
{
public:
    explicit server(const std::string& ip = "127.0.0.1", int port=1337);
    ~server();
    void WriteBuffer(const char* buffer, size_t size) const;

private:
    std::string ip;
    int port;
};
