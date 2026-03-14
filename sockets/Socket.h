#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <iostream>

#define IPV4 AF_INET
#define IPV6 AF_INET6
#define STREAM SOCK_STREAM

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

struct Socket
{
  fd_t socket_fd{};
  fd_t socket_client_fd{};
  struct sockaddr_in socket_server_addr{};
  struct sockaddr_in socket_client_addr{};
  Socket_type type;

  port_t port;

  Socket(family_t family, conn_type_t conn_type, port_t port, iP_t ip, Socket_type type, protocol_t protocol = 0); // default arguments in declarations only

  void Sock_Listen(int clients_in_queue);

  void Sock_accept();
  
};

#endif