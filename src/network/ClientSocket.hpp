#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include <netlink/socket.h>

#include <string>


class ClientSocket {
  public:
    ClientSocket() = delete;
    ClientSocket(NL::Socket *p_socket);

    void send(char const *message) const;
    void send(char const *message, size_t message_len) const;
    void send(std::string const& message) const;
    void send(std::string const& message, size_t message_len) const;

  private:
    NL::Socket *p_socket;
};


#endif // CLIENT_SOCKET_HPP
