#ifndef MANAGEMENT_HPP
#define MANAGEMENT_HPP

#include <string>
#include <vector>

#include "Timetable.hpp"

class Management {
  public:
    Timetable timetable;
    std::vector<std::string> teams_names;
};

#endif // MANAGEMENT_HPP
