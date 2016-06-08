#include "Stadium.hpp"
#include "RaceResult.hpp"
#include "Race.hpp"
#include "Timetable.hpp"

#include "util/logging.hpp"
#include "network/Server.hpp"
#include "network/ClientSocket.hpp"
#include "util/stringutils.hpp"

#include <json.hpp>

#include <vector>
#include <random>
#include <map>
#include <functional>

using json = nlohmann::json;
using MessageHandler = Server::MessageHandler;
using namespace std::placeholders;


// Stadium: constructors

Stadium::Stadium()
        : management_server(5001),
          logger("Stadium") {
    this->management_server.setMessageHandler(std::bind(&Stadium::messageHandler, this, _1, _2));
}

Stadium::Stadium(json const& data)
        : management_server(5001),
          logger("Stadium") {
    this->management_server.setMessageHandler(std::bind(&Stadium::messageHandler, this, _1, _2));
    this->deserialize(data);
}

// Stadium: destructor

Stadium::~Stadium() {}

// Stadium: ISerializable methods

json Stadium::serialize() const {
    json j;

    j["tracks"] = json::array();
    for (Stadium::Track const& t : this->tracks) {
        j["tracks"].push_back(t.serialize());
    }

    return j;
}

void Stadium::deserialize(json const& data) {
    this->tracks.clear();
    for (json const& j : data["tracks"]) {
        Stadium::Track t(j);
        this->tracks.push_back(t);
    }
}

// Stadium: own methods

std::vector<Stadium::Track> Stadium::getTracks() const {
    return this->tracks;
}

void Stadium::addTrack(Stadium::Track const& track) {
    this->tracks.push_back(track);
}

RaceResult Stadium::startRace(Timetable::Entry const& entry) {
    std::random_device rdev;
    std::default_random_engine engine(rdev());
    std::uniform_int_distribution<unsigned int> uniform_dist(5, 20);
    unsigned int duration = uniform_dist(engine);

    Race race(duration, entry.getTarakansInfo());
    
    return race.start();
}

#define UNUSED(x) (void)(x)

void Stadium::messageHandler(std::string const& message,
                             ClientSocket const& client_socket) {
    this->logger.debug("Handling message...");
    this->logger.debug("Message contents: \"" + message + "\"");

    std::map<std::string, MessageHandler> handlers =
            { { "START RACE", std::bind(&Stadium::handleStartRace, this, _1, _2) } };

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

void Stadium::handleStartRace(std::string const& message_body,
                              ClientSocket const& client_socket) {
    UNUSED(client_socket);

    Timetable::Entry entry(json::parse(message_body));

    this->startRace(entry);
}

#undef UNUSED


// Stadium::Track: constructor

Stadium::Track::Track(unsigned int id,
                      unsigned int lanes_count,
                      unsigned int team_quota)
        : id(id),
          lanes_count(lanes_count),
          team_quota(team_quota)
        {}

Stadium::Track::Track(json const& data) {
    this->deserialize(data);
}

// Stadium::Track: destructor

Stadium::Track::~Track() {}

// Stadium::Track: ISerializable methods

json Stadium::Track::serialize() const {
    json j;

    j["id"] = this->id;
    j["lanes_count"] = this->lanes_count;
    j["team_quota"] = this->team_quota;

    return j;
}

void Stadium::Track::deserialize(json const& data) {
    this->id = data["id"];
    this->lanes_count = data["lanes_count"];
    this->team_quota = data["team_quota"];
}

// Stadium::Track: own methods

unsigned int Stadium::Track::getId() const {
    return this->id;
}

unsigned int Stadium::Track::getLanesCount() const {
    return this->lanes_count;
}

unsigned int Stadium::Track::getTeamQuota() const {
    return this->team_quota;
}
