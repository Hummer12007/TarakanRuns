#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP

#include <string>
#include <vector>

#include "ISerializable.hpp"

class Timetable : public ISerializable {
  public:
    class Entry : public ISerializable {
      public:
        ~Entry(void);
        std::string serialize(void);
        void deserialize(std::string const &data);

        unsigned int timestamp;
        unsigned int track_id;
        std::vector<std::string> racers_names;
    };

    ~Timetable(void);
    std::string serialize(void);
    void deserialize(std::string const &data);

    std::vector<Timetable::Entry> entries;
};

#endif // TIMETABLE_HPP
