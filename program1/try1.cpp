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
			getline(myfile, contents);

			if (contents.find("accept") != string::npos) {
				if (contents.find("start") != string::npos) {
					startstate = state;
				}
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

	
	//print tests
	cout << "start state: " << startstate << endl;
	cout << "accept states: " << endl;
    for(list<int>::iterator i = accept_states.begin(); i != accept_states.end(); ++i){
        cout << *i << endl;
    }

    for(multimap<int, string>:: const_iterator i = transitions.begin(); i!= transitions.end();++i){
        cout << i->first << '\t' << i->second << endl;
    }
	

	list<int> nfa;
	list<int> final_state_accept;
	list<int> final_state_reject;

	//nfa.emplace_back(startstate);
	
	multimap<int,string>::iterator i = transitions.find(startstate);
	
	int nfa_iter = nfa.front();

	list<int>::iterator check;
	list<int>::iterator check2;
	list<int>::iterator check3;
	list<int>::iterator check4;
	string type;


	while (input.length() >= 1) {
		for (int j = 0; j < transitions.count(i->first); j++) {
			if (input[0] == i->second[0]) {
				tostate = atoi(((i->second).substr(1)).c_str());
				cout << "going to "<< tostate << " on "<< i->second[0] << " from " << i->first << endl;
				nfa.emplace_back(tostate);
				/*check = find(nfa.begin(), nfa.end(), tostate);
				if (check == nfa.end()) { // tostate isnt in nfa list
					nfa.emplace_back(tostate);
				}*/
				check2 = find(accept_states.begin(), accept_states.end(), tostate);
				check3 = find(final_state_accept.begin(), final_state_accept.end(), tostate);
				check4 = find(final_state_reject.begin(), final_state_reject.end(), tostate);
				/*if ((input.length() == 1 || check2 != accept_states.end()) && check3 == final_state.end()) {
					final_state.emplace_back(tostate);
					cout << "final state comes from " << i->first << " on " << i->second[0] << endl;
				}*/
				if (check2 != accept_states.end()) { // tostate is an accept state
					type = "accept";
					if (check3 == final_state_accept.end()) { // isnt in the final
						final_state_accept.emplace_back(tostate);
					}
				}else {
					type = "reject";
					if (input.length() == 1 && check4 == final_state_reject.end()) {
						final_state_reject.emplace_back(tostate);
					}
				}
			}
			i++;
		}

		i = transitions.find(nfa.front());
		nfa.pop_front();

		/*if (nfa_iter != nfa.back()) {
			nfa_iter++;
			i = transitions.find(nfa_iter);
		} else {
			i = transitions.find(nfa_iter);
		}*/

		if (input.length() > 2) {
			input = input.substr(1);
		} else if (input.length() == 2) {
			input = string(1, input[1]);
		} else {
			input.clear();
		}

	}
	if (final_state_reject.empty() && final_state_accept.empty()) {
		cout << "reject" << endl;
		return 0;
	}

    //print accept or reject
    if (type.compare("accept") == 0) {
    	cout << "accept";
    	for(list<int>::iterator i = final_state_accept.begin(); i != final_state_accept.end(); ++i){
        	cout << " " << *i;
    	}
    }else {
    	cout << "reject";
    	for(list<int>::iterator i = final_state_reject.begin(); i != final_state_reject.end(); ++i){
        	cout << " " << *i;
    	}
    }
    cout << endl;

}