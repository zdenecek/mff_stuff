
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <stack>
#include <memory>


using namespace std;

#include "debug.h"
#include "errors.h"
#include "Function.h"
#include "Expression.h"

bool checkIsAlpha(string s) {
	for (char c : s) {
		if (!isalpha(c)) return false;
	}

	return true;
}


class FunctionDefinitionParser {

public:
	FunctionTable parseDefinitions(istream& input) const {

		FunctionTable table = unordered_map<string, Function>();
		string line;
		while (getline(input, line)) {
			try {

				auto fun = parseDefinition(line);

				table.insert({ fun.name, fun });

			}
			catch (runtime_error& e) {

				if (DEBUG_MESSAGES_ON_INVALID_INPUT)
				{
					cout << "error in: " << line << endl;
				}
				//cerr << e.what() << endl;
				cout << e.what() << endl;

				if (DEBUG_MESSAGES_ON_INVALID_INPUT) cout << endl;
			}
		}

		return table;

	}
private:
	Function parseDefinition(string line) const
	{
		string name;
		string token;
		vector<int> translations;
		istringstream iss(line);
		iss >> name;
		unordered_map<string, int> localvariables = unordered_map<string, int>();

		if (!checkIsAlpha(name)) throw runtime_error(PARSE_ERROR);

		int i = 0;
		while (iss >> token && token != "=") {

			if (localvariables.find(token) != localvariables.end()) {
				//cerr << "Repeated param name" << endl;
				//throw runtime_error("Repeated param name");
				throw runtime_error(DEF_ERROR);
			}

			if (!checkIsAlpha(token)) throw runtime_error(PARSE_ERROR);

			localvariables.insert({ token, i });
			i++;
		}

		if (token != "=") {
			//cerr << "Expected =" << endl;
			//throw runtime_error("Expected =");
			throw runtime_error(PARSE_ERROR);
		}

		if (localvariables.size() == 0) throw runtime_error(PARSE_ERROR);

		while (iss >> token) {
			if (localvariables.find(token) != localvariables.end()) {
				translations.push_back(localvariables[token]);
			}
			else {
				//cerr << "Undefined variable: " + token << endl;
				throw runtime_error(DEF_ERROR);
			}
		}
		return Function(name, move(translations), i);
	}


};