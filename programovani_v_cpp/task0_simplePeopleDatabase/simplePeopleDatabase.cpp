// simplePeopleDatabase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>

using namespace std;

struct PersonEntry
{
public:
	PersonEntry(string &name, int age)
	{
		this->age = age;
		this->name = name;
	}

	string name;
	int age;
};

class SimplePeopleDatabase
{

	vector<PersonEntry> people;

public:
	void print() const
	{
		for (auto person : people)
		{
			cout << "{ name: " << person.name << ", age: " << person.age << " }" << endl;
			//  printf("{ name: %s, age: %d }\n",  person.name.c_str(), person.age);
		}
	}

	void insert(string &name, int age)
	{
		people.emplace_back(name, age);
	}

	void removeName(string &name)
	{
		this->removeIf(
			[name](const PersonEntry &person)
			{
				return person.name == name;
			});
	}
	void removeAge(int age)
	{
		this->removeIf(
			[age](const PersonEntry &person)
			{
				return person.age == age;
			});
	}

	void removeIf(const std::function<bool(const PersonEntry &)> condition)
	{
		people.erase(std::remove_if(
						 people.begin(), people.end(),
						 condition),
					 people.end());
	}

	void sortName()
	{
		this->sort(
			[](const PersonEntry &personA, const PersonEntry &personB)
			{
				return personA.name.compare(personB.name) < 0;
			});
	}
	void sortAge()
	{
		this->sort(
			[](const PersonEntry &personA, const PersonEntry &personB)
			{
				return personA.age < personB.age;
			});
	}

	void sort(const std::function<bool(const PersonEntry &, const PersonEntry &)> comparer)
	{
		stable_sort(people.begin(), people.end(), comparer);
	}
};

bool handle(string &command, SimplePeopleDatabase &db)
{
	istringstream ss(command);

	string directive;
	getline(ss, directive, ' ');

	auto pos = directive.find('_');
	string directivePostFix;
	if (pos != string::npos)
	{
		directivePostFix = directive.substr(1 + pos, directive.length());
		directive = directive.substr(0, pos);
	}

	if (directive == "exit")
		return true;
	else if (directive == "print")
	{
		db.print();
	}
	else if (directive == "insert")
	{
		string name;
		string ageString;
		int age;
		cin >> name;
		cin >> ageString;
		age = stoi(ageString);
		db.insert(name, age);
	}
	else if (directive == "remove")
	{
		if (directivePostFix == "age")
		{
			string ageString;
			cin >> ageString;
			db.removeAge(stoi(ageString));
		}
		if (directivePostFix == "name")
		{
			string name;
			cin >> name;

			db.removeName(name);
		}
	}
	else if (directive == "sort")
	{
		if (directivePostFix == "age")

			db.sortAge();
		if (directivePostFix == "name")
			db.sortName();
	}
	return false;
}

int main()
{
	SimplePeopleDatabase db;

	string line;
	while (cin >> line)
	{

		if (handle(line, db))
			break;
	}
}
