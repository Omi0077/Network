#include <arpa/inet.h>
#include <iostream>

#define IPV4 AF_INET;
#define IPV6 AF_INET6;
#define STREAM SOCK_STREAM;

using fd_t = int;
using port_t = int;
using family_t = typeof(AF_INET);
using family_6t = typeof(AF_INET6);
using conn_type_t = typeof(SOCK_STREAM);
using protocol_t = typeof(int);
using iP_t = typeof(INADDR_ANY);

enum class Socket_type : int
{
  server,
  client
};

class Socket
{
  fd_t socket_fd{};
  fd_t socket_client_fd{};
  struct sockaddr_in socket_server_addr{};
  // struct sockaddr_in socket_client_addr{};
  Socket_type type;

  port_t port;

  Socket(family_t family, conn_type_t conn_type, protocol_t protocol = 0, port_t port, iP_t ip, Socket_type type)
      : port(port), type(type)
  {

    this->socket_server_addr.sin_family = family;
    this->socket_server_addr.sin_port = htons(port);
    this->socket_server_addr.sin_addr.s_addr = ip;
    if (this->type == Socket_type::server)
    {
      socket_fd = socket(family, conn_type, protocol);

      if (bind(socket_fd, (struct sockaddr *)&socket_server_addr, sizeof(socket_server_addr)) < 0)
      {
        std::cerr << "Failed to bind\n";
        exit(EXIT_FAILURE);
      }
    }
    else if (type == Socket_type::client)
    {
      socket_client_fd = socket(family, conn_type, protocol);

      if(connect(socket_client_fd, (struct sockaddr*)&socket_server_addr, sizeof(socket_server_addr)) < 0){
        std::cerr << "Failed to connect to server/n";
        exit(EXIT_FAILURE);
      }
    }
  }

  void Sock_Listen(int clients_in_queue)
  {
    if(this->type == Socket_type::client) return;
    
    if (listen(socket_fd, clients_in_queue) < 0)
    {
      std::cerr << "Failed to listen\n";
      exit(EXIT_FAILURE);
    }

    std::cout << "Socket is listening on " << this->port << "\n";
  }

  
};