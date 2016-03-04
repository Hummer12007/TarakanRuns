#include <functional>
#include <map>
#include <string>
#include <stdlib.h>

#include "json.hpp"
#include "Team.hpp"
#include "Management.hpp"
#include "ISerializable.hpp"
#include "Stadium.hpp"
#include "Timetable.hpp"

int testTeam() {
    Team team;
    std::cout << "\tTesting Team serialization\n";

    std::cout << "\tEnter team's name\n	>>";
    std::string name;
    std::cin >> name;
    team.name = name;

    if (name.empty())
        return 1;

    std::cout << "\tEnter number of Tarakans\n >>";
    int cuc = 0;
    std::cin >> cuc;

    for (int i = 0; i < cuc; i++) {
        std::string cuc_name;
        std::cout << "\tEnter racer's name >> ";
        std::cin >> cuc_name;

        Team::Tarakan t;
        t.name = cuc_name;
        team.tarakans.push_back(t);
    }

    std::cout << "\tSerializing:\n";

    std::string team_serialized = team.serialize();
    std::cout << "\t" << team_serialized << std::endl;

    std::cout << "\tDeserializing:\n";

    team.deserialize(team_serialized);

    std::cout << "\tteam name : " << team.name << "\n\ttarakans : [";

    std::vector<Team::Tarakan> tarakans = team.tarakans;
    cuc = tarakans.size();
    for (int i = 0; i < cuc; i++) {
        if (i > 0)
            std::cout << ", ";
        std::cout << tarakans[i].name;
    }

    std::cout << "]\n\tEnd\n";

    return 0;
}

int testTimetable() {
    std::cout << "Testing Timetable serialization\n";

    Timetable tt;

    int n = rand() % 3 + 1;

    for (int i = 0; i < n; i++) {
        Timetable::Entry te;

        te.timestamp = 1;
        te.track_id = i + 1;

        int n_racers = rand() % 4 + 1;

        for (int j = 0; j < n_racers; j++) {
            std::string racer_name = "racer" + std::to_string(j);

            te.racers_names.push_back(racer_name);
        }

        tt.entries.push_back(te);
    }

    std::cout << "\tSerializing:\n";

    std::string tt_serialized = tt.serialize();
    std::cout << "\t" << tt_serialized << std::endl;

    std::cout << "\tDeserializing:\n";

    tt.deserialize(tt_serialized);

    std::cout << "\tTimetable : [\n";

    std::vector<Timetable::Entry> entries = tt.entries;

    n = entries.size();

    for (int i = 0; i < n; i++) {
        std::cout << "\t\t{timestamp : " << entries[i].timestamp
                  << ", track_id : " << entries[i].track_id << " [\n";

        std::vector<std::string> racers = entries[i].racers_names;
        int n_racers = racers.size();

        for (int j = 0; j < n_racers; j++) {
            std::cout << "\t\t\t" << racers[i] << "\n";
        }

        std::cout << "\t\t] }\n";
    }

    std::cout << "\t]";

    std::cout << "\n\tEnd\n";

    return 0;
}

int testStadium() {
    std::cout << "Testing Stadium serialization\n";

    Stadium st;

    int n = rand() % 4 + 1;

    for (int i = 0; i < n; i++) {
        Stadium::Track tr;

        tr.id = i;
        tr.lanes = rand() % 4 + 1;
        tr.team_quota = rand() % 6 + 1;

        st.tracks.push_back(tr);
    }

    std::cout << "\tSerializing:\n";

    std::string st_serialized = st.serialize();
    std::cout << "\t" << st_serialized << std::endl;

    std::cout << "\tDeserializing:\n";

    st.deserialize(st_serialized);

    std::cout << "\tStadium : [\n";

    std::vector<Stadium::Track> tracks = st.tracks;

    n = tracks.size();

    for (int i = 0; i < n; i++) {
        std::cout << "\t\t{id : " << tracks[i].id
                  << ", lanes : " << tracks[i].lanes
                  << ", team_quota : " << tracks[i].team_quota << " }\n";
    }

    std::cout << "\t]";

    std::cout << "\n\tEnd\n";

    return 0;
}

int main() {
    std::map<std::string, std::function<int(void)>> functions;
    functions["Team"] = testTeam;
    functions["Stadium"] = testStadium;
    functions["Timetable"] = testTimetable;

    std::string s = "Enter class name to test or EXIT to "
                    "exit\n\tTeam\n\tStadium\n\tTimetable\n>>";
    while (true) {
        // std::cout << "\033[1;31mbold red text\033[0m\n";
        std::cout << s;
        std::string test_str;
        std::cin >> test_str;

        if (test_str == "EXIT")
            break;

        if (functions.find(test_str) == functions.end()) {
            std::cout
                << "\t\033[1;31mSomething went wrong! Try again!\033[0m\n";
            continue;
        }

        int err = functions[test_str]();
    }

    std::cout << "Terminated\n";
    return 0;
}
