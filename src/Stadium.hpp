#ifndef STADIUM_HPP
#define STADIUM_HPP
#include <vector>

#include "Track.hpp"
#include "ISerializable.hpp"
class Stadium : public ISerializable<Stadium> {
	public:
		std::vector<Track> tracks;
};
#endif //STADIUM_HPP

