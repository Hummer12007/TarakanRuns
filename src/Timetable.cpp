#include <string>
#include <vector>

#include "json.hpp"
#include "Timetable.hpp"

using json = nlohmann::json;


// Timetable: ISerializable methods

std::string Timetable::serialize( void ) {
    json j;
    
    j["entries"] = json::array();
    for (Timetable::Entry e : this->entries) {
        j["entries"].push_back(e.serialize());
    }
    
    return j.dump();
}

void Timetable::deserialize( std::string& data ) {
    json j = json::parse(data);
    
    this->entries.clear();
    for (std::string s : j["entries"]) {
        Timetable::Entry e = Timetable::Entry::deserialize(s);
        this->entries.push_back(e);
    }
}


// Timetable::Entry: ISerializable methods

std::string Timetable::Entry::serialize( void ) {
    json j;
    
    j["timestamp"] = this->timestamp;
    j["track_id"] = this->track_id;
    j["racers_names"] = json(this->racers_names);
    
    return j.dump();
}

void Timetable::Entry::deserialize( std::string& data ) {
    json j = json::parse(data);
    
    this->timestamp = j["timestamp"];
    this->track_id = j["track_id"];
    this->racers_names.clear();
    for (std::string s : j["racers_names"]) {
        this->racers_names.push_back(s);
    }
}