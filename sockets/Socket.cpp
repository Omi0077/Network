#include <arpa/inet.h>
#include <iostream>
#include "./Socket.h"


  Socket::Socket(family_t family, conn_type_t conn_type, protocol_t protocol = 0, port_t port, iP_t ip, Socket_type type)
      : port(port), type(type)
  {
    // prepare server address
    this->socket_server_addr.sin_family = family;
    this->socket_server_addr.sin_port = htons(port);
    this->socket_server_addr.sin_addr.s_addr = ip;

    if (this->type == Socket_type::server)
    {
      socket_fd = socket(family, conn_type, protocol);
      // bind socket to the server address
      if (bind(socket_fd, (struct sockaddr *)&socket_server_addr, sizeof(socket_server_addr)) < 0)
      {
        std::cerr << "Failed to bind\n";
        exit(EXIT_FAILURE);
      }
    }
    else if (type == Socket_type::client)
    {
      socket_client_fd = socket(family, conn_type, protocol);

      // connect to the server
      if(connect(socket_client_fd, (struct sockaddr*)&socket_server_addr, sizeof(socket_server_addr)) < 0){
        std::cerr << "Failed to connect to server/n";
        exit(EXIT_FAILURE);
      }
    }
  }

  void Socket::Sock_Listen(int clients_in_queue)
  {
    if(this->type == Socket_type::client) return;

    if (listen(socket_fd, clients_in_queue) < 0)
    {
      std::cerr << "Failed to listen\n";
      exit(EXIT_FAILURE);
    }

    std::cout << "Socket is listening on " << this->port << "\n";
  }

  void Socket::Sock_accept(){
    if(this->type == Socket_type::client) return;

    socklen_t client_addr_size = sizeof(socket_client_addr);
    
    // accept fills clent ip and port and returns new socket descriptor to client_fd
    socket_client_fd = accept(socket_fd, (struct sockaddr*)&socket_client_addr, &client_addr_size);
    if(socket_client_fd < 0) {
      std::cerr << "Failed to accept\n";
      exit(EXIT_FAILURE);
    }

    std::cout << "connected by " << inet_ntoa(socket_client_addr.sin_addr) << ":" << ntohs(socket_client_addr.sin_port);
  }