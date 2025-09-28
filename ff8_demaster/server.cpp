#include "server.h"

#include <format>

#include "debug.h"

#if USE_SERVER
server::server(const std::string& ip, int port)
{
    this->ip= ip;
    this->port = port;
}

server::~server()
= default;

#else
server::server(const std::string& ip, int port)
{
    //
}

server::~server()
{
    //
}
#endif

std::string server::GetCurrentDateTime()
{
    const std::time_t now = std::time(nullptr);
    const std::tm* timeinfo = localtime(&now);
    char buffer[80];
    size_t _ = strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", timeinfo);
    return buffer;
}

void server::WriteLog(const std::string& log)
{
    const std::string logMessage = std::format("MAKILOG|{}{}\n", GetCurrentDateTime(), log);
    WriteBuffer(logMessage.c_str(), logMessage.length());
}

void server::WriteBuffer(const char* buffer, const size_t size)
{
#if USE_SERVER
    if(ignoreServer)
        return;
    try
    {
        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip, std::to_string(port));
        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints);
    
        asio::write(socket, asio::buffer(buffer, size));
        socket.shutdown(asio::ip::tcp::socket::shutdown_send);
        socket.close();
    }
    catch (std::exception& e)
    {
        OutputDebug("Failed to write buffer to server: %s\n", e.what());
        if(++retries>MAX_RETRIES)
        {
            ignoreServer = true;
            OutputDebug("Server connection lost, disabling writes for this client.\n");
        }
    }
#endif
}
