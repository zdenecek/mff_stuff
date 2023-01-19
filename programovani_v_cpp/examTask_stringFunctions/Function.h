#pragma once	


#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class Function {

public:
	string name;
	vector<int> translations;
	size_t argc;

	Function(string name, vector<int>&& translations, int argc) : name(name), translations(translations), argc(argc) {

	}
};