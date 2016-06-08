#ifndef TARAKAN_HPP
#define TARAKAN_HPP

#include "ISerializable.hpp"
#include "RandomStrategy.hpp"

#include "util/logging.hpp"

#include <json.hpp>

#include <string>
#include <vector>

using json = nlohmann::json;


class Tarakan : public ISerializable<json> {
  public:
    Tarakan(std::string const& name);
    Tarakan(json const& data);
    ~Tarakan();

    json serialize() const;
    void deserialize(json const& data);

    std::string getName() const;
    std::vector<RandomStrategy> getStrategies() const;

    void addStrategy(RandomStrategy const& strategy);

  private:
    std::string name;
    std::vector<RandomStrategy> strategies;
    Logger logger;
};


#endif // !TARAKAN_HPP
