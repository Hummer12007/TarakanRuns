#ifndef STADIUM_HPP
#define STADIUM_HPP

#include "ISerializable.hpp"
#include "RaceResult.hpp"
#include "Timetable.hpp"

#include "network/ClientSocket.hpp"
#include "network/Server.hpp"
#include "util/logging.hpp"

#include <json.hpp>

#include <vector>

using json = nlohmann::json;


class Stadium : public ISerializable<json> {
  public:
    class Track : public ISerializable<json> {
      public:
        Track(unsigned int id,
              unsigned int lanes_count,
              unsigned int team_quota);
        Track(json const& data);
        ~Track();

        json serialize() const;
        void deserialize(json const& data);

        unsigned int getId() const;
        unsigned int getLanesCount() const;
        unsigned int getTeamQuota() const;

      private:
        unsigned int id;
        unsigned int lanes_count;
        unsigned int team_quota;
    };

    Stadium();
    Stadium(json const& data);
    ~Stadium();

    json serialize() const;
    void deserialize(json const& data);

    std::vector<Stadium::Track> getTracks() const;
    void addTrack(Stadium::Track const& track);

  private:
    RaceResult startRace(Timetable::Entry const& entry);

    void messageHandler(std::string const& message,
                        ClientSocket const& client_socket);

    void handleStartRace(std::string const& message_body,
                         ClientSocket const& client_socket);

    std::vector<Stadium::Track> tracks;
    Server management_server;
    Logger logger;
};

#endif // !STADIUM_HPP
