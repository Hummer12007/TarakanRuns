#define CATCH_CONFIG_MAIN

#include "Team.hpp"
#include "Timetable.hpp"
#include "Stadium.hpp"

#include "catch.hpp"
#include "json.hpp"

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>


// In this UnitTest we check string serialization instead of json object
// serialization. This allows as test the json::dump function as well.


std::string genRandomString(size_t len) {
    std::ostringstream oss;

    for (size_t i = 0; i < len; ++i) {
        char c = rand() % (126 - 33) + 33;
        c = (c == 92 || c == 34) ? c + 1 : c;
        oss << c;
    }

    return oss.str();
}


// Team related

Team genRandomTeam() {
    size_t name_len = rand() % 10 + 6;
    Team team(genRandomString(name_len));

    size_t tarakans_count = rand() % 15;
    for (size_t i = 0; i < tarakans_count; ++i) {
        name_len = rand() % 10 + 6;
        Team::Tarakan tarakan(genRandomString(name_len));
        team.addTarakan(tarakan);
    }

    return team;
}

TEST_CASE( "testing team", "[Team]" ) {
    srand(time(NULL));

    Team team = genRandomTeam();

    std::ostringstream oss;
    oss << "{\"name\":\"" << team.getName() << "\"" << ","
        <<  "\"tarakans\":" << "[";

    std::vector<Team::Tarakan> tarakans = team.getTarakans();
    for (size_t i = 0; i < tarakans.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }

        oss << "{\"name\":\"" << tarakans.at(i).getName() << "\"}";
    }

    oss << "]}";
    std::string team_str_serialized = oss.str();

    SECTION( "testing team serialization" ) {
        REQUIRE( team.serialize().dump() == team_str_serialized );
    }

    SECTION( "testing team deserialization" ) {
        Team team_deserialized(json::parse(team_str_serialized));

        SECTION( "testing deserialized name of the team" ) {
            REQUIRE( team.getName() == team_deserialized.getName() );
        }

        SECTION( "testing size of deserialized vector of tarakans" ) {
            REQUIRE( team.getTarakans().size() ==
                     team_deserialized.getTarakans().size() );
        }

        SECTION( "testing deserialized tarakans names" ) {
            std::vector<Team::Tarakan> tarakans = team.getTarakans();
            std::vector<Team::Tarakan> tarakans_deserialized =
                    team_deserialized.getTarakans();

            size_t tarakans_count = tarakans.size();
            for (size_t i = 0; i < tarakans_count; ++i) {
                REQUIRE( tarakans.at(i).getName() ==
                         tarakans_deserialized.at(i).getName() );
            }
        }
    } 
}


// Timetable related

Timetable genRandomTimetable() {
    Timetable timetable;
    size_t entries_count = rand() % 15;

    for (size_t i = 0; i < entries_count; ++i) {
        unsigned int timestamp = rand() % 49 + 1;
        unsigned int track_id = rand() % 19 + 1;
        Timetable::Entry entry(timestamp, track_id);

        size_t racers_count = rand() % 15;
        for (size_t j = 0; j < racers_count; ++j) {
            entry.addRacerName(genRandomString(rand() % 10 + 6));
        }
    }

    return timetable;
}

TEST_CASE( "testing timetable", "[Timetable]" ) {
    srand(time(NULL));

    Timetable timetable = genRandomTimetable();

    std::ostringstream oss;
    oss << "{\"entries\":[";

    std::vector<Timetable::Entry> entries = timetable.getEntries();
    for (size_t i = 0; i < entries.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }

        oss << "{\"racers_names\":[";

        std::vector<std::string> racers_names = entries.at(i).getRacersNames();
        for (size_t j = 0; j < racers_names.size(); ++j) {
            if (j > 0) {
                oss << ",";
            }

            oss << "\"" << racers_names.at(i);
        }

        oss << "]" << ","
            << "\"timestamp\":"
            <<     std::to_string(entries.at(i).getTimestamp())
            << "\"track_id\":"
            <<     std::to_string(entries.at(i).getTrackId())
            << "}";
    }

    oss << "]}";
    std::string timetable_str_serialized = oss.str();

    SECTION( "testing timetable serialization" ) {
        REQUIRE( timetable.serialize().dump() == timetable_str_serialized );
    }

    SECTION( "testing timetable deserialization" ) {
        Timetable timetable_deserialized(
                json::parse(timetable_str_serialized));

        SECTION( "testing size of deserialized vector of entries" ) {
            REQUIRE( timetable.getEntries().size() ==
                     timetable_deserialized.getEntries().size() );
        }

        SECTION( "testing deserialized entries" ) {
            std::vector<Timetable::Entry> entries = timetable.getEntries();
            std::vector<Timetable::Entry> entries_deserialized =
                    timetable_deserialized.getEntries();

            size_t entries_count = entries.size();
            for (size_t i = 0; i < entries_count; ++i) {
                REQUIRE( entries.at(i).getTrackId() ==
                         entries_deserialized.at(i).getTrackId() );
                REQUIRE( entries.at(i).getTimestamp() ==
                         entries_deserialized.at(i).getTimestamp() );
                
                SECTION( "testing size of deserialized vector of "
                         "racers names" ) {
                    REQUIRE( entries.at(i).getRacersNames().size() ==
                             entries_deserialized.at(i)
                                    .getRacersNames().size() );
                }

                SECTION( "testing deserialized racers names" ) {
                    std::vector<std::string> racers_names =
                            entries.at(i).getRacersNames();
                    std::vector<std::string> racers_names_deserialized =
                            entries_deserialized.at(i).getRacersNames();

                    size_t racers_names_count = racers_names.size();
                    for (size_t i = 0; i < racers_names_count; ++i) {
                        REQUIRE( racers_names.at(i) ==
                                 racers_names_deserialized.at(i) );
                    }
                }
            }
        }
    }
}


// Stadium related

Stadium genRandomStadium() {
    Stadium stadium;
    size_t tracks_count = rand() % 15;

    for (size_t i = 0; i < tracks_count; ++i) {
        unsigned int id = rand() % 18 + 1;
        unsigned int lanes_count = rand() % 12 + 1;
        unsigned int team_quota = rand() % 7 + 1;

        Stadium::Track track(id, lanes_count, team_quota);
        stadium.addTrack(track);
    }

    return stadium;
}

TEST_CASE( "testing stadium", "[Stadium]" ) {
    srand(time(NULL));

    Stadium stadium = genRandomStadium();

    std::ostringstream oss;
    oss << "{\"tracks\":[";

    std::vector<Stadium::Track> tracks = stadium.getTracks();
    for (size_t i = 0; i < tracks.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }

        oss << "{\"id\":"
            <<     std::to_string(tracks.at(i).getId()) << ","
            << "\"lanes_count\":"
            <<     std::to_string(tracks.at(i).getLanesCount()) << ","
            << "\"team_quota\":"
            <<     std::to_string(tracks.at(i).getTeamQuota()) << "}";
    }

    oss << "]}";
    std::string stadium_str_serialized = oss.str();

    SECTION( "testing stadium serialization" ) {
        REQUIRE( stadium.serialize().dump() == stadium_str_serialized );
    }

    SECTION( "testing stadium deserialization" ) {
        Stadium stadium_deserialized(json::parse(stadium_str_serialized));

        SECTION( "testing size of deserialized vector of stadium tracks" ) {
            REQUIRE( stadium.getTracks().size() ==
                     stadium_deserialized.getTracks().size() );
        }

        SECTION( "testing deserialized tracks" ) {
            std::vector<Stadium::Track> tracks = stadium.getTracks();
            std::vector<Stadium::Track> tracks_deserialized =
                    stadium_deserialized.getTracks();

            size_t tracks_count = tracks.size();
            for (size_t i = 0; i < tracks_count; ++i) {
                REQUIRE( tracks.at(i).getId() ==
                         tracks_deserialized.at(i).getId() );
                REQUIRE( tracks.at(i).getLanesCount() ==
                         tracks_deserialized.at(i).getLanesCount() );
                REQUIRE( tracks.at(i).getTeamQuota() ==
                         tracks_deserialized.at(i).getTeamQuota() );
            }
        }
    }
}
