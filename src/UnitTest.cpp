#define CATCH_CONFIG_MAIN

#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>

#include "catch.hpp"
#include "Team.hpp"
#include "Stadium.hpp"
#include "Timetable.hpp"
#include "Broker.hpp"
#include "IMessageable.hpp"

using string = std::string;
using Tarakan = Team::Tarakan;
using Track = Stadium::Track;
using Entry = Timetable::Entry;
using Account = Broker::Account;
using Bet = Broker::Bet;

class VoidMessageable : public IMessageable {
  public:
    VoidMessageable(void){};
    void receive_message(std::string const &message,
                         std::function<void(std::string const &)> &fun){};
    ~VoidMessageable(void){};
};

void rnd_string(string &s, int len) {
    s = "";
    for (int i = 0; i < len; i++) {
        char c = rand() % (126 - 33) + 33;
        c = (c == 92 || c == 34) ? c + 1 : c;
        s += c;
    }
}

void rnd_team(Team &t) {
    t.name = "";
    int len = rand() % 6 + 4;
    rnd_string(t.name, len);

    int tar_num = rand() % 5;
    for (int i = 0; i < tar_num; i++) {
        Tarakan tar;
        len = rand() % 6 + 4;
        rnd_string(tar.name, len);
        t.tarakans.push_back(tar);
    }
}

std::string dbl_to_str(double &d) {
    std::ostringstream o;
    if (std::fmod(d, 1) == 0) {
        o << std::fixed << std::setprecision(1);
    } else {
        o.unsetf(std::ios_base::floatfield);
        o << std::setprecision(std::numeric_limits<double>::digits10);
    }
    o << d;
    return o.str();
}

TEST_CASE("testing team", "[Team]") {
    srand(time(NULL));
    SECTION("testing team serialization") {
        Team t_serialize;
        rnd_team(t_serialize);

        string s;
        s = "{\"name\":\"" + t_serialize.name + "\",\"tarakans\":["; //+
        //+ "{\"name\":\"tarakan\"},{\"name\":\"tarakan\"}" +
        //+ "]}";

        int tar_num = t_serialize.tarakans.size();
        for (int i = 0; i < tar_num; i++) {
            if (i > 0)
                s += ",";
            s += "{\"name\":\"" + t_serialize.tarakans[i].name + "\"}";
        }

        s += "]}";

        SECTION("testing serialized size of tarakans") {
            REQUIRE(t_serialize.tarakans.size() == tar_num);
        }

        REQUIRE(t_serialize.serialize() == s);

        SECTION("testing team deserialization") {
            Team t_deserialize;

            t_deserialize.deserialize(s);

            int tar_num = t_serialize.tarakans.size();

            SECTION("testing deserialized size of tarakans") {
                REQUIRE(t_deserialize.tarakans.size() == tar_num);
            }

            SECTION("testing deserialized name of team") {
                REQUIRE(t_deserialize.name == t_serialize.name);
            }

            SECTION("testing deserialized tarakans names") {
                for (int i = 0; i < tar_num; i++) {
                    REQUIRE(t_deserialize.tarakans[i].name ==
                            t_serialize.tarakans[i].name);
                }
            }
        }
    }
}

void rnd_stadium(Stadium &st) {
    int track_num = rand() % 5;
    for (int i = 0; i < track_num; i++) {
        Track tr;
        int id = rand() % 8 + 1;
        int lanes = rand() % 5 + 1;
        int team_quota = rand() % 3 + 1;
        tr.id = id;
        tr.lanes = lanes;
        tr.team_quota = team_quota;
        st.tracks.push_back(tr);
    }
}

TEST_CASE("testing stadium", "[Stadium]") {
    srand(time(NULL));
    SECTION("testing team serialization") {
        Stadium st_serialize;
        rnd_stadium(st_serialize);

        string s = "{\"tracks\":[";

        for (auto &track : st_serialize.tracks) {
            s += "{\"id\":";
            s += std::to_string(track.id) + ",\"lanes\":";
            s += std::to_string(track.lanes) + ",\"team_quota\":";
            s += std::to_string(track.team_quota) + "}";
            if (&track != &*std::prev(std::end(st_serialize.tracks)))
                s += ",";
        }
        s += "]}";

        /*{"tracks":[{"id":7,"lanes":1,"team_quota":2},{"id":8,"lanes":5
,"team_quota":3},{"id":4,"lanes":2,"team_quota":1},{"id":5,"lanes":5
,"team_quota":1}]}*/

        REQUIRE(st_serialize.serialize() == s);

        SECTION("testing stadium deserialization") {
            Stadium st_deserialize;

            st_deserialize.deserialize(s);

            int tr_num = st_serialize.tracks.size();

            SECTION("testing deserialized size of tracks") {
                REQUIRE(st_deserialize.tracks.size() == tr_num);
            }

            SECTION("testing deserialized tarakans names") {
                for (int i = 0; i < tr_num; i++) {
                    REQUIRE(st_serialize.tracks[i].id ==
                            st_deserialize.tracks[i].id);
                    REQUIRE(st_serialize.tracks[i].lanes ==
                            st_deserialize.tracks[i].lanes);
                    REQUIRE(st_serialize.tracks[i].team_quota ==
                            st_deserialize.tracks[i].team_quota);
                }
            }
        }
    }
}

void rnd_timetable(Timetable &tt) {
    int entry_num = rand() % 5;
    for (int i = 0; i < entry_num; i++) {
        Entry e;
        e.track_id = rand() % 9 + 1;
        e.timestamp = rand() % 19 + 1;
        int racers_num = rand() % 5;
        for (int i = 0; i < racers_num; i++) {
            string str;
            rnd_string(str, rand() % 5 + 3);
            e.racers_names.push_back(str);
        }
        tt.entries.push_back(e);
    }
}

TEST_CASE("testing timetable", "[Timetable]") {
    srand(time(NULL));
    SECTION("testing team serialization") {
        Timetable tt_serialize;
        rnd_timetable(tt_serialize);
        string s = "{\"entries\":[";

        for (auto &e : tt_serialize.entries) {
            s += "{\"racers_names\":[";
            for (auto &racer : e.racers_names) {
                s += "\"" + racer + "\"";
                if (&racer != &*std::prev(std::end(e.racers_names)))
                    s += ",";
            }
            s += "]";
            s += ",\"timestamp\":" + std::to_string(e.timestamp) +
                 ",\"track_id\":" + std::to_string(e.track_id) + "}";
            if (&e != &*std::prev(std::end(tt_serialize.entries)))
                s += ",";
        }

        s += "]}";
        /*{"entries":[{"racers_names":["O)F]#X","`I])BQ8"],"timestamp":19
,"track_id":3}]}*/

        REQUIRE(tt_serialize.serialize() == s);

        SECTION("testing timetable deserialization") {
            Timetable tt_deserialize;

            tt_deserialize.deserialize(s);

            int e_num = tt_serialize.entries.size();

            SECTION("testing deserialized size of tarakans") {
                REQUIRE(tt_deserialize.entries.size() == e_num);
            }

            SECTION("testing deserialized tarakans names") {
                for (int i = 0; i < e_num; i++) {
                    REQUIRE(tt_deserialize.entries[i].track_id ==
                            tt_serialize.entries[i].track_id);
                    REQUIRE(tt_deserialize.entries[i].timestamp ==
                            tt_serialize.entries[i].timestamp);
                    SECTION("testing deserialized racers_names size") {
                        REQUIRE(tt_deserialize.entries[i].racers_names.size() ==
                                tt_serialize.entries[i].racers_names.size());
                    }
                    /////////////////////////////////////////////////////////////
                    SECTION("testing deserialized racers_names") {
                        for (int j = 0;
                             j < tt_deserialize.entries[i].racers_names.size();
                             j++) {
                            REQUIRE(tt_deserialize.entries[i].racers_names[j] ==
                                    tt_serialize.entries[i].racers_names[j]);
                        }
                    }
                }
            }
        }
    }
}

void rnd_double(double &d) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(1, 2);
    d = dis(gen);
}

void rnd_broker(Broker &b) {
    int a_num = rand() % 10;
    int b_num = rand() % 10;

    for (int i = 0; i < a_num; i++) {
        Account a;
        int len = rand() % 5 + 3;
        rnd_string(a.name, len);
        rnd_double(a.balance);
        b.accounts.push_back(a);
    }

    for (int i = 0; i < a_num; i++) {
        Bet bet;
        int len = rand() % 5 + 3;
        rnd_string(bet.client_name, len);

        bet.race_id = rand() % 6;

        len = rand() % 5 + 3;
        rnd_string(bet.racer_name, len);

        rnd_double(bet.amount);
        b.bets.push_back(bet);
    }
}

TEST_CASE("testing broker", "[Broker]") {
    srand(time(NULL));
    SECTION("testing broker serialization") {
        std::shared_ptr<VoidMessageable> sp_vm(new VoidMessageable);
        Broker b_serialize(sp_vm);
        rnd_broker(b_serialize);
        string s = "{\"accounts\":[";

        for (auto &a : b_serialize.accounts) {
            s += "{\"balance\":" + dbl_to_str(a.balance);
            s += ",\"name\":\"" + a.name + "\"}";

            if (&a != &*std::prev(std::end(b_serialize.accounts)))
                s += ",";
        }

        s += "],\"bets\":[";

        for (auto &b : b_serialize.bets) {
            s += "{\"amount\":" + dbl_to_str(b.amount);
            s += ",\"client_name\":\"" + b.client_name + "\"";
            s += ",\"race_id\":" + std::to_string(b.race_id);
            s += ",\"racer_name\":\"" + b.racer_name + "\"}";
            if (&b != &*std::prev(std::end(b_serialize.bets)))
                s += ",";
        }

        s += "]}";

        REQUIRE(b_serialize.serialize() == s);

        SECTION("testing broker deserialization") {
            Broker b_deserialize(sp_vm);

            b_deserialize.deserialize(s);

            int a_num = b_serialize.accounts.size();
            int b_num = b_serialize.bets.size();

            SECTION("testing deserialized size of accounts") {
                REQUIRE(b_deserialize.accounts.size() == a_num);
            }

            SECTION("testing deserialized accounts") {
                for (int i = 0; i < a_num; i++) {
                    REQUIRE(b_deserialize.accounts[i].name ==
                            b_serialize.accounts[i].name);
                    REQUIRE(b_deserialize.accounts[i].balance ==
                            Approx(b_serialize.accounts[i].balance));
                }
            }

            SECTION("testing deserialized size of bets") {
                REQUIRE(b_deserialize.bets.size() == b_num);
            }

            SECTION("testing deserialized bets") {
                for (int i = 0; i < b_num; i++) {
                    REQUIRE(b_deserialize.bets[i].client_name ==
                            b_serialize.bets[i].client_name);
                    REQUIRE(b_deserialize.bets[i].race_id ==
                            b_serialize.bets[i].race_id);
                    REQUIRE(b_deserialize.bets[i].racer_name ==
                            b_serialize.bets[i].racer_name);
                    REQUIRE(b_deserialize.bets[i].amount ==
                            Approx(b_serialize.bets[i].amount));
                }
            }
        }
    }
}
