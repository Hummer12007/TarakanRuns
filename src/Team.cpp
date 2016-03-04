#include <string>
#include <vector>

#include "json.hpp"
#include "Team.hpp"

using json = nlohmann::json;

// Team: destructor

Team::~Team(void) {}

// Team: ISerializable methods

std::string Team::serialize(void) {
    json j;

    j["name"] = this->name;
    j["tarakans"] = json::array();
    for (Team::Tarakan t : this->tarakans) {
        j["tarakans"].push_back(json::parse(t.serialize()));
    }

    return j.dump();
}

void Team::deserialize(std::string const &data) {
    json j = json::parse(data);

    this->name = j["name"];
    std::vector<Team::Tarakan> tarakans;
    this->tarakans.clear();

    for (json o : j["tarakans"]) {
        Team::Tarakan t;
        t.deserialize(o.dump());
        this->tarakans.push_back(t);
    }
}

// Team::Tarakan: destructor

Team::Tarakan::~Tarakan(void) {}

// Team::Tarakan: ISerializable methods

std::string Team::Tarakan::serialize(void) {
    json j;

    j["name"] = this->name;

    return j.dump();
}

void Team::Tarakan::deserialize(std::string const &data) {
    json j = json::parse(data);

    this->name = j["name"];
}
