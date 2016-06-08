#ifndef RACE_HPP
#define RACE_HPP

#include "RaceResult.cpp"
#include "RandomStrategy.hpp"

#include <map>
#include <string>
#include <vector>
#include <utility>


class Race {
  public:
    Race(unsigned int duration,
         std::map<std::string, std::vector<RandomStrategy>> tarakans_info);

    void setDuration(unsigned int duration);
    void addTarakanInfo(std::string const& name,
                        std::vector<RandomStrategy> const& strategies);

    RaceResult start();

  private:
    void initRace();
    void processRace();
    void sortResults();
    void packResults();

    unsigned int duration;
    std::map<std::string, std::vector<RandomStrategy>> tarakans_info;

    // For passing data between functions
    std::map<std::string, RandomStrategy> current_tarakans_info;
    std::map<std::string, unsigned int> dist_positions_map;
    std::vector<std::pair<unsigned int, std::string>> dist_positions;
    std::vector<std::string> positions;
};


#endif // RACE_HPP
