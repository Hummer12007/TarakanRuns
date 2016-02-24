#ifndef STADIUM_HPP
#define STADIUM_HPP


#include <vector>

class Stadium : ISerializable<Stadium::Track> {
    public:
        class Track : ISerializable<Stadium::Track> {
            public:
                unsigned int id;
                unsigned int lanes;
                unsigned int team_quota;
        };
        
        std::vector<Stadium::Track> tracks;
};


#endif // STADIUM_HPP