#ifndef TEAM_HPP
#define TEAM_HPP

#include <string>
#include <vector>

#include "ISerializable.hpp"

class Team : public ISerializable {
  public:
    class Tarakan : public ISerializable {
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
