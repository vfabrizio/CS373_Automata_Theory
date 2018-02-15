#include <iostream>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

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
			myfile >> contents;

			if (contents.find("accept") != string::npos) {
				accept_states.emplace_back(state);
			}
			if (contents.find("start") != string::npos) {
				startstate = state;
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

	/* print tests
	cout << "start state: " << startstate << endl;
    for(list<int>::iterator i = accept_states.begin(); i != accept_states.end(); ++i){
        cout << *i << endl;
    }

    for(multimap<int, string>:: const_iterator i = transitions.begin(); i!= transitions.end();++i){
        cout << i->first << '\t' << i->second << endl;
    }
	*/

	list<int> nfa;
	list<int> final_state;

	nfa.emplace_back(startstate);
	
	multimap<int,string>::iterator i = transitions.find(startstate);
	
	int nfa_iter = nfa.front();

	list<int>::iterator check;
	list<int>::iterator check2;
	list<int>::iterator check3;
	string type;


	while (input.length() >= 1) {
		for (int j = 0; j < transitions.count(i->first); j++) {
			if (input[0] == i->second[0]) {
				tostate = atoi(((i->second).substr(1)).c_str());
				check = find(nfa.begin(), nfa.end(), tostate);
				if (check == nfa.end()) { // tostate isnt in nfa list
					nfa.emplace_back(tostate);
				}
				check2 = find(accept_states.begin(), accept_states.end(), tostate);
				check3 = find(final_state.begin(), final_state.end(), tostate);
				if ((input.length() == 1 || check2 != accept_states.end()) && check3 == final_state.end()) {
					final_state.emplace_back(tostate);
				}
				if (check2 != accept_states.end()) {
					type = "accept";
				}else {
					type = "reject";
				}
			}
			i++;
		}

		if (nfa_iter != nfa.back()) {
			nfa_iter++;
			i = transitions.find(nfa_iter);
		} else {
			i = transitions.find(nfa_iter);
		}

		if (input.length() > 2) {
			input = input.substr(1);
		} else if (input.length() == 2) {
			input = string(1, input[1]);
		} else {
			input.clear();
		}

	}
	if (final_state.empty()) {
		cout << "reject" << endl;
		return 0;
	}

    //print accept or reject
    if (type.compare("accept") == 0) {
    	cout << "accept";
    }else {
    	cout << "reject";
    }
    for(list<int>::iterator i = final_state.begin(); i != final_state.end(); ++i){
        cout << " " << *i;
    }
    cout << endl;

}