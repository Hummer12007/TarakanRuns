#ifndef RACE_RESULT_HPP
#define RACE_RESULT_HPP

#include "ISerializable.hpp"

#include <json.hpp>

#include <vector>
#include <string>

using json = nlohmann::json;


class RaceResult : public ISerializable<json> {
  public:
    RaceResult(std::vector<std::string> const& positions);
    ~RaceResult();

    json serialize() const;
    void deserialize(json const& data);

  private:
    std::vector<std::string> positions;
};


#endif // !RACE_RESULT_HPP
