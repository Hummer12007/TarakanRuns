#ifndef TEAM_HPP
#define TEAM_HPP

#include "ISerializable.hpp"
#include "Tarakan.hpp"
#include "RandomStrategy.hpp"
#include "Timetable.hpp"

#include "network/Client.hpp"
#include "util/logging.hpp"

#include <json.hpp>

#include <string>
#include <vector>

using json = nlohmann::json;


class Team : public ISerializable<json> {
  public:
    Team(std::string const& name);
    Team(json const& data);
    ~Team();

    json serialize() const;
    void deserialize(json const& data);

    std::string getName() const;
    std::vector<Tarakan> getTarakans() const;

    void addTarakan(std::string const& tarakan_name);

    Timetable requestTimetable();
    void registerTeam();
    void deregisterTeam();
    bool registerTarakan(std::string const& tarakan_name,
                         unsigned int track_id,
                         unsigned int timestamp);

  private:
    std::string name;
    std::vector<Tarakan> tarakans;
    Client management_client;
    Logger logger;
};


#endif // !TEAM_HPP
