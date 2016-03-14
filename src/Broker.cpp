#include "Broker.cpp"
#include "json.hpp"

using json = nlohmann::json;

Broker::Broker(IMessageable &master) {
	this->master = master;
}

std::string Broker::process_message(message) {
	// TODO: HREN
}

void Broker::receive_message(std::string message, std::function<void(string)> &fun) {
	fun(this->process_message(message));
}

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

std::string Broker::Account::serialize(void) {
	json j;
	j["id"] = this->id;
	j["balance"] = this->balance;
	return j.dump();
}

std::string Broker::Bet::serialize(void) {
	json j;
	j["client"] = this->client_id;
	j["race"] = this->race_id;
	j["racer"] = this->racer_id;
	j["amount"] = this->amount;
	return j.dump();
}

void Broker::deserialize(std::string const &data) {
	json j = json::parse(data);

	this->accounts.clear();
	this->bets.clear();


	for (json jo : j["accounts"]) {
		Broker::Account acc;
		acc.deserialize(jo.dump());
		this->accounts.push_back(acc);
	}
	for (json jo : j["bets"]) {
		Broker::Bet bet;
		bet.deserialize(jo.dump());
		this->bets.push_back(bet);
	}
}

void Broker::Account::deserialize(std::string const &data) {
	json jo = json::parse("data");

	this->id = jo["id"];
	this->balance = jo["balance"];
}

void Broker::Account::deserialize(std::string const &data) {
	json jo = json::parse("data");

	this->client_id = jo["client"];
	this->race_id = jo["race"];
	this->racer_id = jo["racer"];
	this->amount = jo["amount"];
}

void deserialize(std::string const &data) {

}
