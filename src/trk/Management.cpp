#include "Management.hpp"

#include "network/ClientSocket.hpp"
#include "network/Server.hpp"
#include "util/stringutils.hpp"

#include <json.hpp>
#include <netlink/socket.h>

#include <string>
#include <functional>
#include <map>

#include <cstring>

using MessageHandler = Server::MessageHandler;
using json = nlohmann::json;
using namespace std::placeholders;


Management::Management()
        : timetable(),
          teams_names(),
          team_server(5000),
          team_client("", 0),
          stadium_client("", 0),
          logger("Management") {
    this->team_server.setMessageHandler(std::bind(&Management::messageHandler, this, _1, _2));
}

Management::~Management() {
    this->stop();
}

void Management::start() {
    this->team_server.start();
}

void Management::stop() {
    this->team_server.stop();
}


#define UNUSED(x) (void)(x)

void Management::messageHandler(std::string const& message,
                                ClientSocket const& client_socket) {
    this->logger.debug("Handling message...");
    this->logger.debug("Message contents: \"" + message + "\"");

    std::map<std::string, MessageHandler> handlers =
            { { "REQUEST TIMETABLE", std::bind(&Management::handleTimetableRequest, this, _1, _2) },
              { "REGISTER TEAM", std::bind(&Management::handleTeamRegistration, this, _1, _2) },
              { "DEREGISTER TEAM", std::bind(&Management::handleTeamDeregistration, this, _1, _2) },
              { "REGISTER TARAKAN", std::bind(&Management::handleTarakanRegistration, this,  _1, _2) } };

    for (auto const& kv : handlers) {
        if (startsWith(message, kv.first)) {
            if (message.size() == kv.first.size()) {
                kv.second("", client_socket);
                goto success;
            } else if (message.at(kv.first.size()) != ' ') {
                break;
            } else {
                std::string message_body = message.substr(kv.first.size() + 1);
                kv.second(message_body, client_socket);
                goto success;
            }
        }
    }

    this->logger.error("Invalid message!");
    throw std::invalid_argument("Invalid message: \"" + message + "\"");

    success:
    this->logger.debug("Message has been handled successfully!");
}

void Management::handleTimetableRequest(std::string const& message_body,
                                        ClientSocket const& client_socket) {
    UNUSED(message_body);

    std::string timetable_str_serialized = this->timetable.serialize().dump();
    std::string response = "TIMETABLE " + timetable_str_serialized;

    this->logger.debug("Response message contents: ", escape(response));
    client_socket.send(response);
}

void Management::handleTeamRegistration(std::string const& escaped_team_name,
                                        ClientSocket const& client_socket) {
    std::string team_name = unescape(escaped_team_name);
    char const *response = nullptr;

    if (this->teams_names.find(team_name) ==
            this->teams_names.end()) {
        this->teams_names.insert(team_name);
        this->logger.info("Registered team: ", escaped_team_name);

        response = "TEAM REGISTERED";
    } else {
        this->logger.error("The team named ", escaped_team_name,
                           " is already registered");

        response = "TEAM ALREADY REGISTERED";
    }

    this->logger.debug("Response message contents: ", escape(response));
    client_socket.send(response);
}

void Management::handleTeamDeregistration(std::string const& escaped_team_name,
                                          ClientSocket const& client_socket) {
    std::string team_name = unescape(escaped_team_name);
    char const *response = nullptr;

    if (this->teams_names.find(team_name) !=
            this->teams_names.end()) {
        this->teams_names.erase(team_name);
        this->logger.info("Deregistered team: ", escaped_team_name);

        response = "TEAM DEREGISTERED";
    } else {
        this->logger.error("No team named ", escaped_team_name,
                           " to deregister");

        response = "NO SUCH TEAM";
    }

    this->logger.debug("Response message contents: ", escape(response));
    client_socket.send(response);
}

void Management::handleTarakanRegistration(std::string const& message_body,
                                           ClientSocket const& client_socket) {
    std::istringstream iss(message_body);
    std::string escaped_tarakan_name;
    unsigned int track_id;
    unsigned int timestamp;
    iss >> escaped_tarakan_name >> track_id >> timestamp;

    // QUESTION: should we put tarakan_name into if block?
    std::string tarakan_name = unescape(escaped_tarakan_name);
    char const *response = nullptr;

    if (this->timetable.isFree(track_id, timestamp)) {
        this->timetable.addTarakan(tarakan_name, track_id, timestamp);
        this->logger.info("Registered tarakan: ", escaped_tarakan_name);

        response = "TARAKAN REGISTERED";
    } else {
        this->logger.error("Refused to register the tarakan ",
                           escaped_tarakan_name,
                           " because the track #", track_id,
                           " at timestamp ", timestamp, " is busy");

        response = "TRACK BUSY";
    }

    this->logger.debug("Response message contents: ", escape(response));
    client_socket.send(response);
}
