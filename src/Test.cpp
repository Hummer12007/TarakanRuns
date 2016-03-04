#include "json.hpp"
#include "Team.hpp"
#include "Management.hpp"
#include "ISerializable.hpp"
#include "Stadium.hpp"
#include <string>
#include "Timetable.hpp"
#include "stdio.h"
#include "stdlib.h"
#include <map>
#include <unistd.h>
#include "cstdlib"

typedef int (*FnPtr)();

static inline void printDots(unsigned int seconds) {
	while(seconds--) {
		usleep(100000);
		std::cout << "." << std::flush;
	}
	printf("\n");
} 

int testTeam() {
	Team team;
	std::cout << "\tTesting Team serialization\n";
	
	std::cout << "\tEnter team's name\n	>>";
	std::string name;
	std::cin >> name;
	team.name = name;
	
	if (name == "") return 1;
	
	printf("%s", "\tEnter number of Cucarachas\n	>>");
	int cuc = 0;
	std::cin >> cuc;
	
	for (int i = 0; i < cuc; i++) {
		std::string cuc_name;
		printf("\tEnter racer's name >> ");
		std::cin >> cuc_name;
		
		Team::Tarakan t;
		t.name = cuc_name;
		team.tarakans.push_back(t);
	}
	
	printf("\tSerializing");

	printDots(10);
	
	printf("%s", "\tCompleted!\n");
	
	std::string team_serialized = team.serialize();
	std::cout << "\t" << team_serialized << std::endl;
	
	printf("%s", "\tStarting deserialization");

	printDots(10);
	
	printf("%s", "\tCompleted!\n");

	team.deserialize(team_serialized);
	
	std::cout << "\tteam name : " << team.name << "\n\tcucarachas : [";
	
	std::vector<Team::Tarakan> tarakans = team.tarakans;
	cuc = tarakans.size();
	for (int i = 0; i < cuc; i++) {
		if (i > 0) std::cout << ", ";
		std::cout << tarakans[i].name;
	}
	
	printf("%s", "]\n\tEnd\n");
	
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
			std::string racer_name = "racer1";
			
			te.racers_names.push_back(racer_name);
		}
		
		tt.entries.push_back(te);
	}
	
	printf("%s", "\tSerialising");
	
	printDots(10);
	
	printf("%s", "\tCompleted!\n");
	
	std::string tt_serialized = tt.serialize();
	std::cout << "\t" << tt_serialized << std::endl;
	
	printf("%s", "\tStarting deserialization");

	printDots(10);
	
	printf("%s", "\tCompleted!\n");

	tt.deserialize(tt_serialized);
	
	printf("%s", "\tTimetable : [\n");
	
	std::vector<Timetable::Entry> entries = tt.entries;
	
	n = entries.size();
	
	for (int i = 0; i < n; i++) {
		std::cout << "\t\t{timestamp : " << entries[i].timestamp << ", track_id : " << entries[i].track_id << " [\n";
		
		std::vector<std::string> racers = entries[i].racers_names;
		int n_racers = racers.size();
		
		for (int j = 0; j < n_racers; j++) {
			std::cout << "\t\t\t" << racers[i] << "\n";
		}
		
		printf("%s", "\t\t] }\n");
	}
	
	printf("%s", "\t]");
	
	printf("%s", "\n\tEnd\n");
	
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
	
	printf("%s", "\tSerialising");
	
	printDots(10);
	
	printf("%s", "\tCompleted!\n");
	
	std::string st_serialized = st.serialize();
	std::cout << "\t" << st_serialized << std::endl;
	
	printf("%s", "\tStarting deserialization");
	
	printDots(10);
	
	printf("%s", "\tCompleted!\n");

	st.deserialize(st_serialized);
	
	printf("%s", "\tStadium : [\n");
	
	std::vector<Stadium::Track> tracks = st.tracks;
	
	n = tracks.size();
	
	for (int i = 0; i < n; i++) {
		std::cout << "\t\t{id : " << tracks[i].id << ", lanes : " << tracks[i].lanes 
			<< ", team_quota : " << tracks[i].team_quota << " }\n";
	}
	
	printf("%s", "\t]");
	
	printf("%s", "\n\tEnd\n");
	
	return 0;
}

int main() {
	std::map<std::string, FnPtr> functions; 
	
	{functions["Team"] = testTeam;
	functions["Stadium"] = testStadium;
	functions["Timetable"] = testTimetable;}
	
	char s[] = "Enter class name to test or EXIT to exit\n\tTeam\n\tStadium\n\tTimetable\n>>"; 
	while (true) {
		//std::cout << "\033[1;31mbold red text\033[0m\n";
		printf("%s", s);
		std::string test_str;
		std::cin >> test_str;
		
		if (test_str == "EXIT") break;
		
		if (functions.find(test_str) == functions.end()) {
			std::cout << "\t\033[1;31mSomething went wrong! Try again!\033[0m\n";
			continue;
		}
		
		int err = functions[test_str]();
	}
	
	printf("Terminated\n");
	return 0;
}