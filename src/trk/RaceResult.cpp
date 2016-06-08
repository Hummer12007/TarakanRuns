#include "RaceResult.hpp"

#include <json.hpp>

#include <vector>
#include <string>

using json = nlohmann::json;


// RaceResult: constructors

RaceResult::RaceResult(std::vector<std::string> const& positions)
        : positions(positions)
        {}

// RaceResult: destructors

RaceResult::~RaceResult() {}

// RaceResult: ISerializable methods

json RaceResult::serialize() const {
    json j;

    j["positions"] = json(this->positions);

    return j;
}

void RaceResult::deserialize(json const& data) {
    this->positions.clear();
    for (std::string const& pos : data["positions"]) {
        this->positions.push_back(pos);
    }
}

// RaceResult: own methods
