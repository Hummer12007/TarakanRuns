#include <string>
#include <vector>

#include "json.hpp"
#include "Stadium.hpp"

using json = nlohmann::json;

// Stadium: ISerializable methods

std::string Stadium::serialize(void) {
    json j;

    j["tracks"] = json::array();
    for (Stadium::Track t : this->tracks) {
        j["tracks"].push_back(t.serialize());
    }

    return j.dump();
}

void Stadium::deserialize(std::string &data) {
    json j = json::parse(data);

    this->tracks.clear();
    for (std::string s : j["tracks"]) {
        Stadium::Track t;
        t.deserialize(s);
        this->tracks.push_back(t);
    }
}

// Stadium::Track: ISerializable methods

std::string Stadium::Track::serialize(void) {
    json j;

    j["id"] = this->id;
    j["lanes"] = this->lanes;
    j["team_quota"] = this->team_quota;

    return j.dump();
}

void Stadium::Track::deserialize(std::string &data) {
    json j = json::parse(data);

    this->id = j["id"];
    this->lanes = j["lanes"];
    this->team_quota = j["team_quota"];
}
