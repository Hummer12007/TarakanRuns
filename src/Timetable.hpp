#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP


#include <string>
#include <vector>

#include "ISerializable.hpp"

class Timetable : ISerializable<Timetable> {
    public:
        class Entry : ISerializable<Timetable::Entry> {
            public:
                unsigned int timestamp;
                unsigned int track_id;
                std::vector<std::string> racers_names;
        };
        
        std::vector<Timetable::Entry> entries;
};


#endif // TIMETABLE_HPP
