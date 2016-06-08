#include "Tarakan.hpp"
#include "RandomStrategy.hpp"

#include <json.hpp>

#include <string>
#include <vector>

using json = nlohmann::json;


// Tarakan: constructors

Tarakan::Tarakan(std::string const& name)
        : name(name),
          strategies(),
          logger("Tarakan") {}

Tarakan::Tarakan(json const& data)
        : logger("Tarakan") {
    this->deserialize(data);
}

// Tarakan: destructor

Tarakan::~Tarakan() {}

// Tarakan: ISerializable methods

json Tarakan::serialize() const {
    json j;

    j["name"] = this->name;
    j["strategies"] = json::array();
    //for (auto const& strategy : strategies) {
    for (size_t i = 0; i < strategies.size(); ++i) {
        j.push_back("RandomStrategy");
    }

    return j;
}

void Tarakan::deserialize(json const& data) {
    this->name = data["name"];
    this->strategies.clear();
    //for (auto const& s : data["strategies"]) {
    for (size_t i = 0; i < data["strategies"].size(); ++i) {
        RandomStrategy randomStrategy;
        this->strategies.push_back(randomStrategy);
    }
}

// Tarakan: own methods

std::string Tarakan::getName() const {
    return this->name;
}

std::vector<RandomStrategy> Tarakan::getStrategies() const {
    return this->strategies;
}

void Tarakan::addStrategy(RandomStrategy const& strategy) {
    this->strategies.push_back(strategy);
}
