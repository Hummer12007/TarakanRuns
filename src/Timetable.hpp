#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP


#include <string>
#include <vector>

#include "ISerializable.hpp"

class Timetable : public ISerializable<Timetable> {
    public:
        class Entry : public ISerializable<Timetable::Entry> {
            public:
                unsigned int timestamp;
                unsigned int track_id;
                std::vector<std::string> racers_names;
        };
        
        std::vector<Timetable::Entry> entries;
};


#endif // TIMETABLE_HPP
