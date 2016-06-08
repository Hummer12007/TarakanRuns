#ifndef TIMETABLE_HPP
#define TIMETABLE_HPP

#include "ISerializable.hpp"
#include "RandomStrategy.hpp"
#include "Tarakan.hpp"

#include <json.hpp>

#include <map>
#include <string>
#include <vector>

using json = nlohmann::json;


class Timetable : public ISerializable<json> {
  public:
    class Entry : public ISerializable<json> {
      public:
        Entry(unsigned int timestamp, unsigned int track_id);
        Entry(json const& data);
        ~Entry();

        json serialize() const;
        void deserialize(json const& data);

        unsigned int getTimestamp() const;
        unsigned int getTrackId() const;
        std::map<std::string, std::vector<RandomStrategy>> getTarakansInfo() const;

        void addTarakanInfo(std::string const& name,
                            std::vector<RandomStrategy> const& strategies);

      private:
        unsigned int timestamp;
        unsigned int track_id;
        std::map<std::string, std::vector<RandomStrategy>> tarakans_info;
    };

    Timetable();
    Timetable(json const& data);
    ~Timetable();

    json serialize() const;
    void deserialize(json const& data);

    std::vector<Timetable::Entry> getEntries() const;

    void addEntry(Timetable::Entry const& entry);
    void addTarakan(Tarakan const& tarakan,
                    unsigned int track_id,
                    unsigned int timestamp);

    bool isFree(unsigned int track_id, unsigned int timestamp) const;

  private:
    std::vector<Timetable::Entry> entries;
};


#endif // !TIMETABLE_HPP
