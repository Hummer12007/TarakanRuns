#include "Client.hpp"
#include "ClientSocket.hpp"

#include "util/logging.hpp"

#include <netlink/socket.h>
#include <netlink/socket_group.h>

#include <string>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <chrono>


Client::Client(std::string const& host_name, unsigned int host_port)
        : host_name(host_name),
          host_port(host_port),
          connection_mutex(),
          logger("Client") {
    this->logger.debug("Creating Client object...");
    this->logger.debug("Passed hostName: \"", host_name, "\"");
    this->logger.debug("Passed hostPort: ", host_port);

    this->connected = false;
    this->is_listening = false;

    try {
        NL::init();

        this->p_read_cmd = new OnRead(this);
        this->p_disconnect_cmd = new OnDisconnect(this);

        this->p_socket_group = new NL::SocketGroup;

        this->p_socket_group->setCmdOnRead(p_read_cmd);
        this->p_socket_group->setCmdOnDisconnect(p_disconnect_cmd);
    } catch (NL::Exception const& e) {
        throw std::runtime_error("Couldn't initialize client. " +
                                 std::string(e.what()));
    }

    this->logger.debug("Client object created ["
                       "hostName=\"", host_name, "\"", ", "
                       "hostPort=", host_port,
                       "]");
}

Client::~Client() {
    this->stopListening();

    delete p_socket_group;

    delete p_disconnect_cmd;
    delete p_read_cmd;

    this->logger.debug("Client object destroyed");
}

bool Client::connect() {
    this->connection_mutex.lock();

    if (this->connected) {
        this->connection_mutex.unlock();
        return true;
    }

    this->logger.info("Establishing connection...");

    try {
        for (size_t i = 0; i < this->p_socket_group->size(); ++i) {
            this->p_socket_group->remove(i);
        }

        this->p_socket_group->add(
                new NL::Socket(this->host_name, this->host_port));

        this->startListening();

        this->connected = true;
    } catch (NL::Exception const& e) {
        this->connected = false;
        this->connection_mutex.unlock();
        throw std::runtime_error("Couldn't connect to server. " +
                                 std::string(e.what()));
    } catch (std::exception const& e) {
        this->connected = false;
        this->connection_mutex.unlock();
        throw std::runtime_error("Couldn't connect to server. " +
                                 std::string(e.what()));
    } catch (...) {
        this->connected = false;
        this->connection_mutex.unlock();
        throw std::runtime_error("Couldn't connect to server "
                                 "with unknown reason.");
    }

    this->logger.info("Connected!");

    return this->connected;
}

void Client::disconnect() {
    if (!this->connected) {
        return;
    }

    this->logger.info("Tearing down connection...");

    try {
        for (size_t i = 0; i < this->p_socket_group->size(); ++i) {
            this->p_socket_group->get(i)->disconnect();
            this->p_socket_group->remove(i);
        }

        this->stopListening();
    } catch (NL::Exception const& e) {
        throw std::runtime_error("An error occured while disconnecting. " +
                                 std::string(e.what()));
    } catch (...) {
        throw std::runtime_error("An error occured while disconnecting "
                                 "with unknown reason.");
    }

    this->logger.info("Disconnected!");

    this->connected = false;
    this->connection_mutex.unlock();
}

bool Client::isConnected() const {
    return this->connected;
}

std::string Client::getHostName() const {
    return this->host_name;
}

unsigned int Client::getHostPort() const {
    return this->host_port;
}

void Client::setHostName(std::string const& host_name) {
    this->host_name = host_name;
}

void Client::setHostPort(unsigned int host_port) {
    this->host_port = host_port;
}

bool Client::sendMessage(std::string const& message) {
    this->logger.debug("Sending the message...");
    this->logger.debug("Message contents: \"" + message + "\"");

    try {
        if (!this->connected) {
            throw std::runtime_error("Cannot send message when "
                                     "client isn't connected!");
        }

        this->stopListening();

        if (message.size() > MAX_MESSAGE_SIZE + 1) {
            throw std::logic_error("Message length is too large.");
        }
        this->p_socket_group->get(0)->send(
                message.c_str(), message.size() + 1);

        this->startListening();
    } catch (NL::Exception const& e) {
        this->connected = false;
        std::cerr << "Error! Sending message failed. "
                  << e.what() << std::endl;
    } catch (std::exception const& e) {
        this->connected = false;
        std::cerr << "Error! Sending message failed. "
                  << e.what() << std::endl;
    }

    this->logger.debug("Message has been sent");

    return this->connected;
}

size_t Client::getInboxSize() const {
    return this->received_messages.size();
}

std::vector<std::string> Client::getInbox() const {
    return this->received_messages;
}

void Client::clearInbox() {
    this->received_messages.clear();
}

void Client::setMessageHandler(MessageHandler const& msg_handler) {
    this->p_read_cmd->setMessageHandler(msg_handler);
}


#define UNUSED(x) (void)(x)

Client::OnRead::OnRead(Client *p_client)
        : p_client(p_client),
          msg_handler(nullptr),
          logger("Client::OnRead") {}

void Client::OnRead::exec(NL::Socket *p_socket,
                          NL::SocketGroup *p_group,
                          void *p_reference) {
    UNUSED(p_group);
    UNUSED(p_reference);

    this->logger.debug("Reading from the established connection stream...");

    try {
        char buffer[MAX_MESSAGE_SIZE];
        buffer[MAX_MESSAGE_SIZE - 1] = '\0';

        this->logger.debug("Reading from socket...");
        p_socket->read(buffer, MAX_MESSAGE_SIZE - 1);
        std::string message = std::string(buffer);
        this->logger.debug("Message contents: \"" + message + "\"");

        if (this->msg_handler == nullptr) {
            this->logger.debug("Pushing the message to the queue...");
            this->p_client->received_messages.push_back(message);
        } else {
            ClientSocket client_socket(p_socket);

            this->logger.debug("Handling the message with a callback...");
            this->msg_handler(message, client_socket);
        }
    } catch (...) {
        std::cerr << "Error! Couldn't receive message!" << std::endl;
    }

    this->logger.debug("Reading completed");
}

void Client::OnRead::setMessageHandler(MessageHandler const& msg_handler) {
    this->msg_handler = msg_handler;
}

Client::OnDisconnect::OnDisconnect(Client *p_client)
        : p_client(p_client),
          logger("Client::OnDisconnect") {}

void Client::OnDisconnect::exec(NL::Socket *p_socket,
                                NL::SocketGroup *p_group,
                                void *p_reference) {
    UNUSED(p_socket);
    UNUSED(p_group);
    UNUSED(p_reference);

    this->logger.debug("Received a disconnecting signal from server");

    this->p_client->disconnect();
}

#undef UNUSED


void Client::listener() {
    try {
        while (this->is_listening) {
            this->p_socket_group->listen(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (...) {
        this->is_listening = false;
    }
}

void Client::startListening() {
    if (!this->is_listening) {
        this->is_listening = true;
        this->p_listening_thread = new std::thread(&Client::listener, this);
    }
}

void Client::stopListening() {
    if (this->is_listening) {
        this->is_listening = false;
        this->p_listening_thread->join();
        delete this->p_listening_thread;
    }
}
