#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

typedef struct config {
	int state;
	string str;
}config;

int main(int argc, char *argv[]) {
	ifstream myfile;
	
	string filename;
	string input;

	if (argc == 3) {
		filename = argv[1];
		input = argv[2];
	} 
	else {
		cout << "error" << endl;
	}

	myfile.open(filename);

	string contents;
	int startstate = 0;
	int state = 0;
	int tostate = 0;
	char trans;

	list<int> accept_states;
	multimap<int, string> transitions;

	myfile >> contents;

	while (!myfile.eof()) {
		if (contents.compare("state") == 0) {
			myfile >> state;
			getline(myfile, contents);

			if (contents.find("acceptstart") != string::npos || contents.find("start\taccept") != string::npos || contents.find("accept\tstart") != string::npos) {
				startstate = state;
				accept_states.emplace_back(state);
			} else if (contents.find("start") != string::npos) {
				startstate = state;
			} else if (contents.find("accept") != string::npos) {
				accept_states.emplace_back(state);
			}

		} else if (contents.compare("transition") == 0) {
			myfile >> state;
			myfile >> trans;
			myfile >> tostate;

			contents = trans + to_string(tostate);
			transitions.insert(pair<int,string>(state, contents));
		} else {
			cout << "Error reading file" << endl;
			return 0;
		}

		myfile >> contents;
	}

	//cout << "fin parse" << endl;

	/*//print tests
	cout << "start state: " << startstate << endl;
	cout << "accept states: " << endl;
    for(list<int>::iterator i = accept_states.begin(); i != accept_states.end(); ++i){
        cout << *i << endl;
    }

    for(multimap<int, string>:: const_iterator i = transitions.begin(); i!= transitions.end();++i){
        cout << i->first << '\t' << i->second << endl;
    }*/

	vector<config> nfa;
	nfa.push_back({startstate, input});
	multimap<int, string>::iterator i;

	list<int> end_states;
	list<int> output_states;

	int current_state;
	string current_string;

	list<int>::iterator check;
	list<int>::iterator check2;

	while (!nfa.empty()) {
		current_state = nfa.front().state;
		current_string = nfa.front().str;

		//cout << "state: " << current_state << " string: " << current_string << endl;

		i = transitions.find(current_state);

		for (int j = 0; j < transitions.count(current_state); j++) {
			//if there is a match, add to vector
			//put in state we are going to and the new string

			if ((i->second)[0] == current_string[0]) {
				nfa.push_back({stoi((i->second).substr(1)), current_string.substr(1)});
			}

			i++;
		}

		if (nfa.front().str.length() == 0) {
			check2 = find(end_states.begin(), end_states.end(), current_state);
			if (check2 == end_states.end()) {
				end_states.push_back(current_state);
			}
		}
		nfa.erase(nfa.begin());
	}

	list<int>::iterator output_itr;
	list<int>::iterator end_itr;


	for (end_itr = end_states.begin(); end_itr != end_states.end(); end_itr++) {
		check = find(accept_states.begin(), accept_states.end(), *end_itr);
		check2 = find(output_states.begin(), output_states.end(), *end_itr);
		if (check != accept_states.end() && check2 == output_states.end()) {
			//if an accept state and not in output already
			output_states.push_back(*end_itr);
		}
	}

	if (output_states.empty()) {
		cout << "reject";
		for (end_itr = end_states.begin(); end_itr != end_states.end(); end_itr++) {
			cout << " " << *end_itr;
		}
		cout << endl;
	} else {
		cout << "accept";
		for (output_itr = output_states.begin(); output_itr != output_states.end(); output_itr++) {
			cout << " " << *output_itr;
		}
		cout << endl;
	}


}