#include <string>
#include <vector>

#include "json.hpp"
#include "Team.hpp"

using json = nlohmann::json;

// Team: ISerializable methods

std::string Team::serialize(void) {
    json j;

    j["name"] = this->name;
    j["tarakans"] = json::array();
    for (Team::Tarakan t : this->tarakans) {
        j["tarakans"].push_back(t.serialize());
    }

    return j.dump();
}

void Team::deserialize(std::string &data) {
    json j = json::parse(data);

    this->name = j["name"];
    std::vector<Team::Tarakan> tarakans();
    this->tarakans.clear();

    for (std::string s : j["tarakans"]) {
        Team::Tarakan t;
        t.deserialize(s);
        this->tarakans.push_back(t);
    }
}

// Team::Tarakan: ISerializable methods

std::string Team::Tarakan::serialize(void) {
    json j;

    j["name"] = this->name;

    return j.dump();
}

void Team::Tarakan::deserialize(std::string &data) {
    json j = json::parse(data);

    this->name = j["name"];
}
