#ifndef BROKER_HPP
#define BROKER_HPP
#include <vector>
#include <string>
#include <map>

#include "IMessageable.hpp"
#include "ISerializable.hpp"

class Broker : public ISerializable<Broker>, IMessageable {
public:
	Broker(IMessageable &master);
    IMessageable master;
    class Broker::Account : public ISerializable<Broker::Account> {
	std::string serialize(void);
	void deserialize(std::string const &data);
	~ISerializable();
    public:
        std::string id;
        double balance;
    };
    class Broker::Bet : public ISerializable<Broker::Bet> {
    public:
        std::string client_id;
        std::string race_id;
        std::string racer_id;
        double amount;
    };
    std::vector<Broker::Account> accounts;
    std::vector<Broker::Bet> bets;
    std::map<std::string, IMessageable> client_handles;
};

#endif //BROKER_HPP