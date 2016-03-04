#ifndef STADIUM_HPP
#define STADIUM_HPP

#include <string>
#include <vector>

#include "ISerializable.hpp"

class Stadium : public ISerializable<Stadium> {
  public:
    class Track : public ISerializable<Stadium::Track> {
      public:
        ~Track(void);
        std::string serialize(void);
        void deserialize(std::string const &data);

        unsigned int id;
        unsigned int lanes;
        unsigned int team_quota;
    };

    ~Stadium(void);
    std::string serialize(void);
    void deserialize(std::string const &data);

    std::vector<Stadium::Track> tracks;
};

#endif // STADIUM_HPP
