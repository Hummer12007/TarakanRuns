#include "Server.hpp"
#include "ClientSocket.hpp"

#include "util/logging.hpp"

#include <netlink/socket.h>
#include <netlink/socket_group.h>

#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>


Server::Server(unsigned int port)
        : port(port),
          logger("Server") {
    this->logger.debug("Creating Server object...");
    this->logger.debug("Passed port: ", port);

    this->running = false;
    this->is_listening = false;

    try {
        NL::init();

        this->p_accept_cmd = new OnAccept(this);
        this->p_read_cmd = new OnRead(this);
        this->p_disconnect_cmd = new OnDisconnect(this);

        this->p_socket_group = new NL::SocketGroup();

        this->p_socket_group->setCmdOnAccept(this->p_accept_cmd);
        this->p_socket_group->setCmdOnRead(this->p_read_cmd);
        this->p_socket_group->setCmdOnDisconnect(this->p_disconnect_cmd);
    } catch (NL::Exception const& e) {
        this->logger.error("Initialization error: network issues");
        throw std::runtime_error(e.what());
    } catch (std::exception const& e) {
        this->logger.error("Initialization error: system issues");
        throw std::runtime_error(e.what());
    } catch (...) {
        this->logger.error("Initialization error: unknown reason");
        throw std::runtime_error("unknown reason");
    }

    this->logger.debug("Server object created ["
                       "port=", port,
                       "]");
}

Server::~Server() {
    this->stopListening();

    delete this->p_socket_group;

    delete this->p_disconnect_cmd;
    delete this->p_read_cmd;
    delete this->p_accept_cmd;

    this->logger.debug("Server object destroyed");
}

bool Server::start() {
    if (this->running) {
        return true;
    }

    this->logger.info("Starting server...");

    try {
        this->logger.debug("Internal this->socket_group->size(): ",
                           this->p_socket_group->size());

        // FIXME: Dangerous `i < m_socketGroup.size()`?
        // NOTE: this->socket_group->remove(i) likely decrements
        // this->socket_group->size()
        for (size_t i = 0; i < this->p_socket_group->size(); ++i) {
            this->p_socket_group->remove(i);
        }

        this->p_socket_group->add(new NL::Socket(this->port));

        this->startListening();

        this->running = true;
    } catch (NL::Exception const& e) {
        this->running = false;
        this->logger.error("Start error: network issues");
        throw std::runtime_error(e.what());
    } catch (std::exception const& e) {
        this->running = false;
        this->logger.error("Start error: system issues");
        throw std::runtime_error(e.what());
    } catch (...) {
        this->running = false;
        this->logger.error("Start error: unknown reason");
        throw std::runtime_error("unknown reason");
    }

    this->logger.info("Server started!");

    return this->running;
}

void Server::stop() {
    if (!this->running) {
        return;
    }

    this->logger.info("Stopping server...");

    try {
        for (size_t i = 0; i < this->p_socket_group->size(); ++i) {
            this->p_socket_group->remove(i);
        }

        this->stopListening();
    } catch (NL::Exception const& e) {
        this->logger.error("Stop error: network issues");
        throw std::runtime_error(e.what());
    } catch (std::exception const& e) {
        this->logger.error("Stop error: system issues");
        throw std::runtime_error(e.what());
    } catch (...) {
        this->logger.error("Stop error: unknown reason");
        throw std::runtime_error("unknown reason");
    }

    this->logger.info("Server stopped!");

    this->running = false;
}

bool Server::isRunning() const {
    return this->running;
}

unsigned int Server::getPort() const {
    return this->port;
}

size_t Server::getConnectionsCount() const {
    return std::max(0, (int) this->p_socket_group->size() - 1);
}

// TODO: Report to Liubomyr that this member function doesn't work as intended
// because servers by definition are only able to receive data, not send.
#if 0
bool Server::sendMessage(std::string const& text, unsigned int index) {
    try {
        if (!m_running) {
            throw std::runtime_error("Cannot send message when "
                                     "server isn't running!");
        }

        this->stopListening();

        if (text.size() > MAX_MESSAGE_SIZE + 1) {
            throw std::logic_error("Message length is too large.");
        }
        m_socketGroup->get(index)->send(text.c_str(), text.size() + 1);

        this->startListening();
    } catch (NL::Exception e) {
        m_running = false;
        std::cerr << "Error! Sending message failed. "
                  << e.what() << std::endl;
    } catch (const std::exception& e) {
        m_running = false;
        std::cerr << "Error! Sending message failed. "
                  << e.what() << std::endl;
    }

    return m_running;
}
#endif

size_t Server::getInboxSize() const {
    return this->received_messages.size();
}

std::vector<std::string> Server::getInbox() const {
    return this->received_messages;
}

void Server::clearInbox() {
    this->received_messages.clear();
}


void Server::setMessageHandler(MessageHandler const& msg_handler) {
    this->p_read_cmd->setMessageHandler(msg_handler);
}


#define UNUSED(x) (void)(x)

Server::OnAccept::OnAccept(Server *p_server)
        : p_server(p_server),
          logger("Server::OnAccept") {}

void Server::OnAccept::exec(NL::Socket *p_socket,
                            NL::SocketGroup *p_group,
                            void *p_reference) {
    UNUSED(p_reference);

    try {
        NL::Socket *new_connection = p_socket->accept();
        p_group->add(new_connection);
    } catch (NL::Exception const& e) {
        this->logger.error("Accept error: network issues");
        throw std::runtime_error(e.what());
    } catch (std::exception const& e) {
        this->logger.error("Accept error: system issues");
        throw std::runtime_error(e.what());
    } catch (...) {
        //std::cerr << "Error! Couldn't add connection!" << std::endl;
        this->logger.error("Accept error: unknown reason");
        throw std::runtime_error("unknown reason");
    }

    this->logger.debug("Connection accepted");
}

Server::OnRead::OnRead(Server *p_server)
        : p_server(p_server),
          msg_handler(nullptr),
          logger("Server::OnRead") {}

void Server::OnRead::exec(NL::Socket *p_socket,
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
            this->p_server->received_messages.push_back(message);
        } else {
            ClientSocket client_socket(p_socket);

            this->logger.debug("Handling the message with a callback...");
            this->msg_handler(message, client_socket);
        }
    } catch (NL::Exception const& e) {
        this->logger.error("Read error: network issues");
        throw std::runtime_error(e.what());
    } catch (std::exception const& e) {
        this->logger.error("Read error: system issues");
        throw std::runtime_error(e.what());
    } catch (...) {
        this->logger.error("Read error: unknown reason");
        throw std::runtime_error("unknown reason");
    }

    this->logger.debug("Reading completed");
}

void Server::OnRead::setMessageHandler(MessageHandler const& msg_handler) {
    this->msg_handler = msg_handler;
}

Server::OnDisconnect::OnDisconnect(Server *p_server)
        : p_server(p_server),
          logger("Server::OnDisconnect") {}

void Server::OnDisconnect::exec(NL::Socket *p_socket,
                                NL::SocketGroup *p_group,
                                void *p_reference) {
    UNUSED(p_reference);

    try {
        p_group->remove(p_socket);
        delete p_socket;
    } catch (NL::Exception const& e) {
        this->logger.error("Disconnect error: network issues");
        throw std::runtime_error(e.what());
    } catch (std::exception const& e) {
        this->logger.error("Disconnect error: system issues");
        throw std::runtime_error(e.what());
    } catch (...) {
        //std::cerr << "Error! Couldn't dop connection!" << std::endl;
        this->logger.error("Disconnect error: unknown reason");
        throw std::runtime_error("unknown reason");
    }

    this->logger.debug("Client disconnected");
}

#undef UNUSED


void Server::listener() {
    try {
        // TODO: Ask Liubomyr for his intent here
        while (this->is_listening) {
            this->p_socket_group->listen(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (...) {
        // FIXME: Ignore exceptions?
        this->is_listening = false;
    }
}

void Server::startListening() {
    if (!this->is_listening) {
        this->is_listening = true;
        this->p_listening_thread = new std::thread(&Server::listener, this);
    }
}

void Server::stopListening() {
    if (this->is_listening) {
        this->is_listening = false;
        this->p_listening_thread->join();
        delete this->p_listening_thread;
    }
}
