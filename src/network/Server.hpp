#ifndef SERVER_HPP
#define SERVER_HPP

#include "ClientSocket.hpp"

#include "util/logging.hpp"

#include <netlink/socket.h>
#include <netlink/socket_group.h>

#include <functional>
#include <vector>
#include <string>
#include <thread>


#define MAX_MESSAGE_SIZE 32768

class Server {
  public:
    typedef std::function<
            void(std::string const&, ClientSocket const&)> MessageHandler;

    Server(unsigned int port);
    ~Server();

    bool start();
    void stop();

    bool isRunning() const;
    unsigned int getPort() const;
    size_t getConnectionsCount() const;

    //bool sendMessage(std::string const& text, unsigned int index);
    size_t getInboxSize() const;
    std::vector<std::string> getInbox() const;
    void clearInbox();

    void setMessageHandler(MessageHandler const& msg_handler);

  private:
    class OnAccept : public NL::SocketGroupCmd {
      public:
        OnAccept(Server *p_server);
        virtual ~OnAccept() {}
        void exec(NL::Socket *p_socket,
                  NL::SocketGroup *p_group,
                  void *p_reference);

      private:
        Server *p_server;
        Logger logger;
    }; friend OnAccept;

    class OnRead : public NL::SocketGroupCmd {
      public:
        OnRead(Server *p_server);
        virtual ~OnRead() {}
        void exec(NL::Socket *p_socket,
                  NL::SocketGroup *p_group,
                  void *p_reference);
        void setMessageHandler(MessageHandler const& msg_handler);

      private:
        Server *p_server;
        MessageHandler msg_handler;
        Logger logger;
    }; friend OnRead;

    class OnDisconnect : public NL::SocketGroupCmd {
      public:
        OnDisconnect(Server *p_server);
        virtual ~OnDisconnect() {}
        void exec(NL::Socket *p_socket,
                  NL::SocketGroup *p_group,
                  void *p_reference);

      private:
        Server *p_server;
        Logger logger;
    }; friend OnDisconnect;

    void listener();
    void startListening();
    void stopListening();
    bool is_listening;
    std::thread *p_listening_thread;

    bool running;
    unsigned int const port;

    std::vector<std::string> received_messages;

    NL::SocketGroup *p_socket_group;
    OnAccept *p_accept_cmd;
    OnRead *p_read_cmd;
    OnDisconnect *p_disconnect_cmd;
    Logger logger;
};


#endif // !SERVER_HPP
