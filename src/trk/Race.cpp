#include "Race.hpp"
#include "RaceResult.hpp"
#include "RandomStrategy.hpp"

#include <map>
#include <string>
#include <vector>
#include <random>
#include <utility>


Race::Race(unsigned int duration,
           std::map<std::string, std::vector<RandomStrategy>> tarakans_info)
        : duration(duration),
          tarakans_info(tarakans_info)
        {}

void Race::setDuration(unsigned int duration) {
    this->duration = duration;
}

void Race::addTarakanInfo(std::string const& name,
                          std::vector<RandomStrategy> const& strategies) {
    if (this->tarakans_info.find(name) == this->tarakans_info.end()) {
        tarakans_info[name] = strategies;
    } else {
        throw std::runtime_error(
                "Couldn't add tarakan info for the existing name");
    }
}

RaceResult Race::start() {
    this->initRace();
    this->processRace();
    this->sortResults();
    this->packResults();

    //return { this->positions };
    RaceResult result(this->positions);
    return result;
}

void Race::initRace() {
    std::random_device rdev;
    std::default_random_engine engine(rdev());

    this->current_tarakans_info.clear();

    for (auto const& kv : this->tarakans_info) {
        std::uniform_int_distribution<size_t>
                uniform_dist(0, kv.second.size() - 1);
        current_tarakans_info[kv.first] = kv.second.at(uniform_dist(engine));
    }
}

void Race::processRace() {
    this->dist_positions_map.clear();

    for (auto const& kv : this->current_tarakans_info) {
        dist_positions_map[kv.first] = 0;
    }

    for (unsigned int i = 0; i < this->duration; ++i) {
        for (auto& kv : this->current_tarakans_info) {
            dist_positions_map[kv.first] += kv.second.exec();
        }
    }
}

void Race::sortResults() {
    this->dist_positions.clear();

    for (auto const& kv : this->dist_positions_map) {
        dist_positions.push_back({ kv.second, kv.first });
    }

    std::sort(dist_positions.begin(), dist_positions.end());
}

void Race::packResults() {
    this->positions.clear();

    for (auto const& p : dist_positions) {
        positions.push_back(p.second);
    }
}
