#include "Team.hpp"
#include "Tarakan.hpp"
#include "Timetable.hpp"
#include "RandomStrategy.hpp"

#include "util/stringutils.hpp"

#include <json.hpp>

#include <string>
#include <set>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <sstream>

#include <cstring>

using json = nlohmann::json;


// Team: constructors

Team::Team(std::string const& name)
        : name(name),
          tarakans(),
          management_client("127.0.0.1", 5000),
          logger("Team")
        {}

Team::Team(json const& data)
        : management_client("127.0.0.1", 5000),
          logger("Team") {
    this->deserialize(data);
}

// Team: destructor

Team::~Team() {}

// Team: ISerializable methods

json Team::serialize() const {
    json j;

    j["name"] = this->name;
    j["tarakans"] = json::array();
    for (Tarakan const& t : this->tarakans) {
        j["tarakans"].push_back(t.serialize());
    }

    return j;
}

void Team::deserialize(json const& data) {
    this->name = data["name"];
    this->tarakans.clear();

    for (json const& j : data["tarakans"]) {
        Tarakan t(j);
        this->tarakans.push_back(t);
    }
}

// Team: own methods

std::string Team::getName() const {
    return this->name;
}

std::vector<Tarakan> Team::getTarakans() const {
    return this->tarakans;
}

void Team::addTarakan(std::string const& tarakan_name) {
    Tarakan tarakan(tarakan_name);
    this->tarakans.push_back(tarakan);
}


Timetable Team::requestTimetable() {
    this->logger.debug("Connecting to the Management server...");
    this->management_client.connect();

    this->logger.debug("Sending a timetable request message...");
    std::string sending_message = "REQUEST TIMETABLE";
    this->logger.debug("Timetable request message contents: ",
                       escape(sending_message));
    this->management_client.sendMessage(sending_message);

    while (this->management_client.getInboxSize() == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    Timetable timetable;

    for (std::string const& message : this->management_client.getInbox()) {
        if (startsWith(message, "TIMETABLE ")) {
            this->logger.info("Got timetable!");

            std::string timetable_str_serialized =
                    message.substr(std::strlen("TIMETABLE "));
            timetable.deserialize(json::parse(timetable_str_serialized));

            break;
        } else {
            throw std::runtime_error("Invalid message: " + escape(message));
        }
    }

    this->management_client.clearInbox();
    this->management_client.disconnect();

    return timetable;
}

void Team::registerTeam() {
    this->logger.debug("Connecting to the Management server...");
    this->management_client.connect();

    std::string escaped_name = escape(this->name);

    this->logger.debug("Sending a registration message...");
    std::string sending_message = "REGISTER TEAM " + escaped_name;
    this->logger.debug("Team registration message contents: ",
                       escape(sending_message));
    this->management_client.sendMessage(sending_message);

    while (this->management_client.getInboxSize() == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (std::string const& message : this->management_client.getInbox()) {
        if (message.compare("TEAM REGISTERED") == 0) {
            this->logger.info("Team ", escaped_name,
                              " successfully registered!");
            break;
        } else if (message.compare("TEAM ALREADY REGISTERED") == 0) {
            this->logger.error("Could not register the team with the name ",
                               escaped_name,
                               ": this name is already registered");
            break;
        } else {
            throw std::runtime_error("Invalid message: " + escape(message));
        }
    }

    this->management_client.clearInbox();
    this->management_client.disconnect();
}

void Team::deregisterTeam() {
    this->logger.debug("Connecting to the Management server...");
    this->management_client.connect();

    std::string escaped_name = escape(this->name);

    this->logger.debug("Sending a deregistration message...");
    std::string sending_message = "DEREGISTER TEAM " + escaped_name;
    this->logger.debug("Team deregistration message contents: ",
                       escape(sending_message));
    this->management_client.sendMessage(sending_message);

    while (this->management_client.getInboxSize() == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (std::string const& message : this->management_client.getInbox()) {
        if (message.compare("TEAM DEREGISTERED") == 0) {
            this->logger.info("Team ", escaped_name,
                              " successfully deregistered!");
            break;
        } else if (message.compare("NO SUCH TEAM") == 0) {
            this->logger.error("Could not deregister the team with the name ",
                               escaped_name,
                               ": this name does not exist");
            break;
        } else {
            throw std::runtime_error("Invalid message: " + escape(message));
        }
    }

    this->management_client.clearInbox();
    this->management_client.disconnect();
}

bool Team::registerTarakan(std::string const& tarakan_name,
                           unsigned int track_id,
                           unsigned int timestamp) {
    this->logger.debug("Connecting to the Management server...");
    this->management_client.connect();

    std::string escaped_tarakan_name = escape(tarakan_name);

    this->logger.debug("Sending a registration message...");
    std::ostringstream oss;
    oss << "REGISTER TARAKAN"
        << " " << escaped_tarakan_name
        << " " << track_id
        << " " << timestamp;
    std::string sending_message = oss.str();

    this->logger.debug("Tarakan registration message contents: ",
                       escape(sending_message));
    this->management_client.sendMessage(sending_message);

    while (this->management_client.getInboxSize() == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    bool success = false;

    for (std::string const& message : this->management_client.getInbox()) {
        if (message.compare("TARAKAN REGISTERED") == 0) {
            this->logger.info("Tarakan ", escaped_tarakan_name,
                              " successfully registered!");
            success = true;
            break;
        } else if (message.compare("TRACK BUSY") == 0) {
            this->logger.error("Could not register the tarakan ",
                               escaped_tarakan_name, " ",
                               "on the track #", track_id, " "
                               "at timestamp ", timestamp,
                               ": there is another tarakan registered ",
                               "on that track at that time");
            success = false;
            break;
        } else {
            throw std::runtime_error("Invalid message: " + escape(message));
        }
    }

    this->management_client.clearInbox();
    this->management_client.disconnect();

    return success;
}
