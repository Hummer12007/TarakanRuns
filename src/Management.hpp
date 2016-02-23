#ifndef MANAGEMENT_HPP
#define MANAGEMENT_HPP
#include <vector>

#include "ISerializable.hpp"
#include "Stadium.hpp"
#include "Team.hpp"
#include "Timetable.hpp"
class Management {
	public:
		Timetable timetable;
		std::vector<Team> teams;
		Stadium stadium;
};
#endif //MANAGEMENT_HPP
