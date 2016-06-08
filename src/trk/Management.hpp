#ifndef MANAGEMENT_HPP
#define MANAGEMENT_HPP

#include "Timetable.hpp"

#include "network/ClientSocket.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "util/logging.hpp"

#include <string>
#include <set>


class Management {
  public:
    Management();
    ~Management();

    void start();
    void stop();

  private:
    void messageHandler(std::string const& message,
                        ClientSocket const& client_socket);

    void handleTimetableRequest(std::string const& message_body,
                                ClientSocket const& client_socket);
    void handleTeamRegistration(std::string const& escaped_team_name,
                                ClientSocket const& client_socket);
    void handleTeamDeregistration(std::string const& escaped_team_name,
                                  ClientSocket const& client_socket);
    void handleTarakanRegistration(std::string const& message_body,
                                   ClientSocket const& client_socket);

    Timetable timetable;
    std::set<std::string> teams_names;

    Server team_server;
    Client team_client;
    Client stadium_client;
    Logger logger;
};

#endif // !MANAGEMENT_HPP
