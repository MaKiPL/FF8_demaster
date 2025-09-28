#pragma once
#if USE_SERVER
#include <asio.hpp>
#endif
#include <string>

class server
{
public:
    explicit server(const std::string& ip = "127.0.0.1", int port=1337);
    ~server();
    static std::string GetCurrentDateTime();
    void WriteLog(const std::string& log);
    void WriteBuffer(const char* buffer, size_t size);

private:
    std::string ip;
    int port;

    const int MAX_RETRIES = 5;
    int retries = 0;
    bool ignoreServer = false;
};
