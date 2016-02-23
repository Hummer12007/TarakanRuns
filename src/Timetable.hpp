#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP
#include <vector>

#include "ISerializable.hpp"
#include "Track.hpp"
#include "Tarakan.hpp"
class Timetable : public ISerializable<Timetable> {
	public:
		class Entry : public ISerializable<Timetable::Entry> {
			public:
				unsigned int timestamp;
				Track track;
				std::vector<Tarakan> racers;
		};
		std::vector <Timetable::Entry> entries;
};
#endif //TIMETABLE_HPP

