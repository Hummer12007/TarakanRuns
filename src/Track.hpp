#ifndef TRACK_HPP
#define TRACK_HPP
#include "ISerializable.hpp"

class Track : public ISerializable<Track> {
	public:
		int id;
		int lanes;
		int team_quota;
};
#endif //TRACK_HPP

