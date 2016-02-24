#ifndef TEAM_HPP
#define TEAM_HPP


#include <string>
#include <vector>

#include "ISerializable.hpp"

class Team : ISerializable<Team> {
    public:
        class Tarakan : ISerializable<Team::Tarakan> {
            public:
                std::string name;
        };
        
        std::string name;
        std::vector<Team::Tarakan> tarakans;
};


#endif // TEAM_HPP
