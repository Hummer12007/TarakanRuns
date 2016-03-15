#include <string>
#include <vector>

#include "json.hpp"
#include "Broker.hpp"

using json = nlohmann::json;

Broker::Broker(std::shared_ptr<IMessageable> const &master) {
    this->master = master;
}

std::string Broker::process_message(std::string const &message) {
    // TODO: HREN
    return "HREN";
}

void Broker::receive_message(std::string const &message,
                             std::function<void(std::string const &)> &fun) {
    fun(this->process_message(message));
}

// Broker: destructor

Broker::~Broker(void) {}

// Broker: ISerializable methods

std::string Broker::serialize(void) {
    json j;

    j["accounts"] = json::array();
    for (Broker::Account acc : this->accounts) {
        j["accounts"].push_back(json::parse(acc.serialize()));
    }

    j["bets"] = json::array();
    for (Broker::Bet bet : this->bets) {
        j["bets"].push_back(json::parse(bet.serialize()));
    }

    return j.dump();
}

void Broker::deserialize(std::string const &data) {
    json j = json::parse(data);

    this->accounts.clear();
    this->bets.clear();

    for (json o : j["accounts"]) {
        Broker::Account acc;
        acc.deserialize(o.dump());
        this->accounts.push_back(acc);
    }

    for (json o : j["bets"]) {
        Broker::Bet bet;
        bet.deserialize(o.dump());
        this->bets.push_back(bet);
    }
}

// Broker::Account: destructor

Broker::Account::~Account(void) {}

// Broker::Account: ISerializable methods

std::string Broker::Account::serialize(void) {
    json j;

    j["name"] = this->name;
    j["balance"] = this->balance;

    return j.dump();
}

void Broker::Account::deserialize(std::string const &data) {
    json j = json::parse(data);

    this->name = j["name"];
    this->balance = j["balance"];
}

// Broker::Bet: destructor

Broker::Bet::~Bet(void) {}

// Broker::Bet: ISerializable methods

std::string Broker::Bet::serialize(void) {
    json j;

    j["client_name"] = this->client_name;
    j["race_id"] = this->race_id;
    j["racer_name"] = this->racer_name;
    j["amount"] = this->amount;

    return j.dump();
}

void Broker::Bet::deserialize(std::string const &data) {
    json j = json::parse(data);

    this->client_name = j["client_name"];
    this->race_id = j["race_id"];
    this->racer_name = j["racer_name"];
    this->amount = j["amount"];
}
