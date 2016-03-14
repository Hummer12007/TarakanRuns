#ifndef BROKER_HPP
#define BROKER_HPP

#include <string>
#include <vector>
#include <map>

#include "ISerializable.hpp"
#include "IMessageable.hpp"

class Broker : public ISerializable, IMessageable {
  public:
    class Account : public ISerializable {
      public:
        std::string serialize(void);
        void deserialize(std::string const &data);
        ~Account(void);

        std::string name;
        double balance;
    };

    class Bet : public ISerializable {
      public:
        std::string serialize(void);
        void deserialize(std::string const &data);
        ~Bet(void);

        std::string client_name;
        unsigned int race_id;
        std::string racer_name;
        double amount;
    };

    Broker(IMessageable &master);
    std::string process_message(std::string &message);

    std::string serialize(void);
    void deserialize(std::string const &data);
    ~Broker(void);

    void receive_message(std::string &message,
                         std::function<void(std::string)> &fun);

    std::shared_ptr<IMessageable> master;
    std::vector<Broker::Account> accounts;
    std::vector<Broker::Bet> bets;
    std::map<std::string, std::shared_ptr<IMessageable>> client_handles;
};

#endif // BROKER_HPP
