#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ClientSocket.hpp"

#include "util/logging.hpp"

#include <netlink/socket.h>
#include <netlink/socket_group.h>

#include <functional>
#include <string>
#include <thread>
#include <mutex>


#define MAX_MESSAGE_SIZE 32768

class Client {
  public:
    typedef std::function<
            void(std::string const&, ClientSocket const&)> MessageHandler;

    Client(std::string const& host_name, unsigned int host_port);
    ~Client();

    bool connect();
    void disconnect();

    bool isConnected() const;
    std::string getHostName() const;
    unsigned int getHostPort() const;
    void setHostName(std::string const& host_name);
    void setHostPort(unsigned int host_port);

    bool sendMessage(std::string const& message);
    size_t getInboxSize() const;
    std::vector<std::string> getInbox() const;
    void clearInbox();

    void setMessageHandler(MessageHandler const& msg_handler);

  private:
    class OnRead : public NL::SocketGroupCmd {
      public:
        OnRead(Client *p_client);
        virtual ~OnRead() {}
        void exec(NL::Socket *p_socket,
                  NL::SocketGroup *p_group,
                  void *p_reference);
        void setMessageHandler(MessageHandler const& msg_handler);

      private:
        Client *p_client;
        MessageHandler msg_handler;
        Logger logger;
    }; friend OnRead;

    class OnDisconnect : public NL::SocketGroupCmd {
      public:
        OnDisconnect(Client *p_client);
        virtual ~OnDisconnect() {}
        void exec(NL::Socket *p_socket,
                  NL::SocketGroup *p_group,
                  void *p_reference);

      private:
        Client *p_client;
        Logger logger;
    }; friend OnDisconnect;

    void listener();
    bool is_listening;
    void startListening();
    void stopListening();

    std::string host_name;
    unsigned int host_port;

    bool connected;
    std::thread *p_listening_thread;
    std::recursive_mutex connection_mutex;

    std::vector<std::string> received_messages;

    NL::SocketGroup *p_socket_group;
    OnRead *p_read_cmd;
    OnDisconnect *p_disconnect_cmd;
    Logger logger;
};


#endif // !CLIENT_HPP
