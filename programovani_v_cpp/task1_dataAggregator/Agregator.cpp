// Agregator.cpp : This file contains the 'main' function. Program execution begins and ends there.
// this code is ugly

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;



union ColumnValue {
	int i;
	string s;
	double d;
};

using row = ColumnValue[];

class ColumnType {

public: 
	virtual ColumnValue saveValue(string &value) = 0;

	static shared_ptr<ColumnType> MakeType(string& type) {

		if (type == "int") return make_shared<IntColumnType>();
		if (type == "double") return  make_shared<DoubleColumnType>();
		if (type == "string") return  make_shared<StringColumnType>();
	}
};

class IntColumnType : public ColumnType {

	ColumnValue saveValue(string& value) override {
		return { 1 };
	}
	

};
class DoubleColumnType : public ColumnType {

	ColumnValue saveValue(string& value) override {
	
		return { 1 };
	}
	

};
class StringColumnType : public ColumnType {

	ColumnValue saveValue(string& value) override {
		return { 1 };

	}


};


class Table {
public:
	vector<shared_ptr<ColumnType>> types;
	vector<string> names;
};

int main()
{
	string word;
	string line;
	stringstream ss;
	
	Table table;

	// parse headers
	cin >> line;

	ss = stringstream(line);
	while (!ss.eof()) {
		getline(ss, word, ';');
		table.names.push_back(word);
	}

	// parse types
	cin >> line;
	ss = stringstream(line);
	while (!ss.eof()) {
		getline(ss, word, ';');
		table.names.push_back(ColumnType::MakeType(word));
	}


	//parse number of lines
	cin >> line;
	int nlines = stoi(line);

	//parse lines

	for (size_t i = 0; i < nlines; i++)
	{

	}

}
