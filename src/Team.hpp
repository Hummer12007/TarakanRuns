#ifndef TEAM_HPP
#define TEAM_HPP

#include <string>
#include <vector>

#include "ISerializable.hpp"

class Team : public ISerializable<Team> {
  public:
    class Tarakan : public ISerializable<Team::Tarakan> {
      public:
        ~Tarakan(void);
        std::string serialize(void);
        void deserialize(std::string const &data);

        std::string name;
    };

    ~Team(void);
    std::string serialize(void);
    void deserialize(std::string const &data);

    std::string name;
    std::vector<Team::Tarakan> tarakans;
};

#endif // TEAM_HPP
