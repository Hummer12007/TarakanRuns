#ifndef TEAM_HPP
#define TEAM_HPP
#include <vector>

#include "ISerializable.hpp"
#include "Tarakan.hpp"

class Team : public ISerializable <Team> {
	public:
		std::string id;
		std::vector<Tarakan> tarakans;
};
#endif //TEAM_HPP

