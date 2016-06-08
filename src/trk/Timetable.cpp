#include "Timetable.hpp"
#include "Team.hpp"
#include "RandomStrategy.hpp"

#include <json.hpp>

#include <vector>
#include <string>

using json = nlohmann::json;


// Timetable: constructors

Timetable::Timetable() {}

Timetable::Timetable(json const& data) {
    this->deserialize(data);
}

// Timetable: destructor

Timetable::~Timetable() {}

// Timetable: ISerializable methods

json Timetable::serialize() const {
    json j;

    j["entries"] = json::array();
    for (Timetable::Entry const& e : this->entries) {
        j["entries"].push_back(e.serialize());
    }

    return j;
}

void Timetable::deserialize(json const& data) {
    this->entries.clear();
    for (json const& j : data["entries"]) {
        Timetable::Entry e(j);
        this->entries.push_back(e);
    }
}

// Timetable: own methods

std::vector<Timetable::Entry> Timetable::getEntries() const {
    return this->entries;
}

void Timetable::addEntry(Timetable::Entry const& entry) {
    this->entries.push_back(entry);
}

void Timetable::addTarakan(Tarakan const& tarakan,
                           unsigned int track_id,
                           unsigned int timestamp) {
    for (auto& entry : this->entries) {
        if (track_id == entry.getTrackId() &&
                timestamp == entry.getTimestamp()) {
            entry.addTarakanInfo(tarakan.getName(), tarakan.getStrategies());
            return;
        }
    }

    Entry new_entry(timestamp, track_id);
    this->entries.push_back(new_entry);
    this->entries.at(this->entries.size() - 1)
                 .addTarakanInfo(tarakan.getName(), tarakan.getStrategies());
}

bool Timetable::isFree(unsigned int track_id, unsigned int timestamp) const {
    for (auto const& entry : this->entries) {
        if (track_id == entry.getTrackId() &&
                timestamp == entry.getTimestamp()) {
            return (entry.getTarakansInfo().size() < 5);
        }
    }

    return true;
}


// Timetable::Entry: constructors

Timetable::Entry::Entry(unsigned int timestamp, unsigned int track_id)
        : timestamp(timestamp), track_id(track_id) {}

Timetable::Entry::Entry(json const& data) {
    this->deserialize(data);
}

// Timetable::Entry: destructor

Timetable::Entry::~Entry() {}

// Timetable::Entry: ISerializable methods

#define UNUSED(x) (void)(x)

json Timetable::Entry::serialize() const {
    json j;

    j["timestamp"] = this->timestamp;
    j["track_id"] = this->track_id;
    //j["tarakans_info"] = json(this->tarakans_info);

    j["tarakans_info"] = json::object();
    for (auto const& kv : this->tarakans_info) {
        json a = json::array();
        for (auto const& strategy : kv.second) {
            UNUSED(strategy);
            a.push_back("RandomStrategy");
        }

        j["tarakans_info"][kv.first] = a;
    }

    return j;
}

void Timetable::Entry::deserialize(json const& data) {
    this->timestamp = data["timestamp"];
    this->track_id = data["track_id"];

    this->tarakans_info.clear();
    json j_trks_info = data["tarakans_info"];

    for (json::iterator it = j_trks_info.begin();
            it != j_trks_info.end();
            ++it) {
        std::vector<RandomStrategy> strategies;
        for (auto const& strategy : it.value()) {
            UNUSED(strategy);
            RandomStrategy randomStrategy;
            strategies.push_back(randomStrategy);
        }
        //std::vector<RandomStrategy> strategies = kv.second;
        this->tarakans_info[it.key()] = strategies;
    }
}

#undef UNUSED

// Timetable::Entry: own methods

unsigned int Timetable::Entry::getTimestamp() const {
    return this->timestamp;
}

unsigned int Timetable::Entry::getTrackId() const {
    return this->track_id;
}

std::map<std::string, std::vector<RandomStrategy>>
Timetable::Entry::getTarakansInfo() const {
    return this->tarakans_info;
}

void Timetable::Entry::addTarakanInfo(
        std::string const& name,
        std::vector<RandomStrategy> const& strategies) {
    this->tarakans_info[name] = strategies;
}
