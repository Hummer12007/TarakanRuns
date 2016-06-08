#include "util/MultiCallHandler.hpp"
#include "Management.hpp"
#include "Team.hpp"

#include <cstring>
#include <stdexcept>
#include <thread>
#include <chrono>


static void set_logging_verbosity(char const *str_verbosity);

static int management_main(int argc, char **argv);
static int team_main(int argc, char **argv);


int main(int argc, char **argv) {
    MultiCallHandler mcHandler("tarakans");

    mcHandler.addFunction("management", management_main);
    mcHandler.addFunction("team", team_main);

    return mcHandler.exec(argc, argv);
}


static void set_logging_verbosity(char const *str_verbosity) {
    unsigned int verbosity = 0;

    if (str_verbosity[0] != '-') {
        throw std::invalid_argument(
                "str_verbosity must be of form -v[v]...");
    }

    for (char c : std::string(str_verbosity + 1)) {
        if (c != 'v') {
            throw std::invalid_argument(
                    "str_verbosity must be of form -v[v]...");
        }

        ++verbosity;
    }

    Logger::setGlobalFormatType(verbosity >= 2 ? VERBOSE_FORMAT:
                                /* else */       NORMAL_FORMAT);
    Logger::setGlobalLogLevel(verbosity == 1 ? INFO_LOG :
                              verbosity == 2 ? INFO_LOG :
                              /* else */       DEBUG_LOG);
}


static int management_main(int argc, char **argv) {
    if (argc > 2) {
        throw std::invalid_argument(
                "This program must be called with either "
                "zero or one argument!");
    }

    if (argc == 2) {
        set_logging_verbosity(argv[1]);
    }

    Management management;
    management.start();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

static int team_main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        throw std::invalid_argument(
                "This program must be called with either "
                "zero or one argument!");
    }

    if (argc == 3) {
        set_logging_verbosity(argv[2]);
    }

    Team *team = new Team(std::string(argv[1]));
    team->registerTeam();
    std::cout << "Team registered!" << std::endl;

    team->registerTarakan("Alfred", 0, 100);
    team->registerTarakan("Peter", 0, 150);
    team->registerTarakan("Rudolf", 0, 150);
    std::cout << "Tarakans registered!" << std::endl;

    Timetable timetable = team->requestTimetable();
    std::cout << "Timetable requested!" << std::endl;
    std::cout << timetable.serialize().dump() << std::endl;

    team->deregisterTeam();
    std::cout << "Team deregistered!" << std::endl;
    delete team;

    return 0;
}
