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

void server::WriteBuffer(const char* buffer, const size_t size) const
{
#if USE_SERVER
    asio::io_context io_context;
    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(ip, std::to_string(port));
    asio::ip::tcp::socket socket(io_context);
    asio::connect(socket, endpoints);
    
    asio::write(socket, asio::buffer(buffer, size));
    socket.shutdown(asio::ip::tcp::socket::shutdown_send);
    socket.close();
#endif
}
