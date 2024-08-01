#include "server.h"

#include "debug.h"

#if USE_SERVER
server::server(const std::string& ip, int port)
{
    this->ip= ip;
    this->port = port;
}

server::~server()
{

}
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
