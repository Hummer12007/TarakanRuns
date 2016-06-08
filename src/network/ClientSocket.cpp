#include "ClientSocket.hpp"

#include <netlink/core.h>
#include <netlink/socket.h>

#include <stdexcept>
#include <string>

#include <cstring>

NL::Protocol const TCP = NL::Protocol::TCP;
NL::SocketType const CLIENT = NL::SocketType::CLIENT;


ClientSocket::ClientSocket(NL::Socket *p_socket) {
    if (p_socket->protocol() == TCP && p_socket->type() == CLIENT) {
        this->p_socket = p_socket;
    } else {
        throw std::invalid_argument(
                "Socket must use TCP protocol and be of CLIENT type!");
    }
}


void ClientSocket::send(char const *message) const {
    p_socket->send(message, std::strlen(message) + 1);
}

void ClientSocket::send(char const *message, size_t message_len) const {
    p_socket->send(message, message_len);
}

void ClientSocket::send(std::string const& message) const {
    this->send(message.c_str());
}

void ClientSocket::send(std::string const& message, size_t message_len) const {
    this->send(message.c_str(), message_len);
}
