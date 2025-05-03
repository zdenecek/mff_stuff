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
#include "FunctionDefinitionParser.h"


using FunctionTable = unordered_map<string, Function>;

// For some reason, I could not move this class away. Always ended up with some cryptic symbol error for errors.h, even if I tried ifndef, pragma once, I could make it work
class ExpressionParser {

public:


	void evaluateExpressions(istream& input, ostream& output, const FunctionTable& context)  const {

		string line;

		while (getline(input, line))
		{

			try {
				output << this->evaluateExpression(line, context) << endl;
			}
			catch (runtime_error& e) {

				if (DEBUG_MESSAGES_ON_INVALID_INPUT)
				{
					cout << "error in: " << line << endl;
				}

				output << e.what() << endl;

				if (DEBUG_MESSAGES_ON_INVALID_INPUT) cout << endl;
			}
		}

	}

	string evaluateExpression(string line, const FunctionTable& context) const {

		try {
			auto expression = parseExpression(line);
			return expression->evaluate(context);
		}
		catch (runtime_error& e) {
			throw;
		}


	}

	unique_ptr<Expression> parseExpression(string line) const {

		// This is our very simple automaton, there is only one explicit state, for the comma, the rest is implicit

		stringstream ss(line);

		stack<unique_ptr<FunctionalExpression>> stk;

		char c;
		bool commaDisallowed = true;
		while ((c = ss.get()) != -1)
		{

			if (c == ' ') continue;
			if (c == ',') {
				if (commaDisallowed) throw runtime_error(PARSE_ERROR);
				commaDisallowed = true;
				continue;
			}

			// follow parsing of a function/literal, after which we jump out OR are prepared for next argument


			if (c == '"') {
				// parse literal

				if (stk.size() == 0) {
					//throw runtime_error("Invalid expression: top level literal");
					throw runtime_error(PARSE_ERROR);
				}

				if (!commaDisallowed) throw runtime_error(PARSE_ERROR); // comma was actually required

				stringstream lit;
				while ((c = ss.get()) != '"') {
					lit << c;
				}
				stk.top()->arguments.push_back(make_unique<ConstantExpression>(ConstantExpression(lit.str())));
				commaDisallowed = false;
			}
			else if (isalpha(c)) {
				// parse function call

				if (stk.size() > 0 && !commaDisallowed) throw runtime_error(PARSE_ERROR); // comma was actually required


				stringstream name;
				name << c;
				while ((c = ss.get()) != '(') {
					if (!isalpha(c)) {
						throw runtime_error("parse error");
						//throw runtime_error("invalid function name");
					}
					name << c;
				}

				stk.push(make_unique<FunctionalExpression>(FunctionalExpression(name.str())));
				// cannot have comma as start
				commaDisallowed = true;
			}
			else if (c == ')') {


				if (stk.size() == 0) {
					throw runtime_error("parse error");
					//throw runtime_error("Invalid expression: unopened )");
				}
				if (commaDisallowed && stk.top()->arguments.size() > 0) {
					// Comma is disallowed either because
					// The arguments have just started, ie foo() - we permit that OR
					// there has been a comma after an argument, ie foo("","",) - we disallow that 
					throw runtime_error("parse error");
					//throw runtime_error("Invalid expression: trailing comma");
				}
				else if (stk.size() == 1) {
					if (ss.peek() == -1) break;
					throw runtime_error("parse error");
					//throw runtime_error("Invalid trailing characters");
				}
				else {
					auto top = move(stk.top());
					stk.pop();
					stk.top()->arguments.push_back(move(top));
				}
				commaDisallowed = false;
			}
			else {
				throw runtime_error("parse error");
				//throw runtime_error("invalid character");
			}

		}
		if (stk.size() != 1) throw runtime_error("Invalid expression");
		return move(stk.top());
	}

};

int run(string defFile, string callFile) {

	ifstream defStream;
	defStream.open(defFile);

	if (!defStream.is_open()) {
		cerr << "Could not open file: " << defFile << endl;
		return 1;
	}

	ifstream callStream;
	callStream.open(callFile);

	if (!callStream.is_open()) {
		cerr << "Could not open file: " << callFile << endl;
		return 1;
	}

	FunctionDefinitionParser defParser;
	FunctionTable table = defParser.parseDefinitions(defStream);

	ExpressionParser exprParser;
	exprParser.evaluateExpressions(callStream, cout, table);

	return 0;
}


int mainpp(vector<string>&& args) {

	if (args.size() != 2) {
		cerr << "Invalid number of arguments" << endl;
		return 1;
	}

	string defFile = args[0];
	string callFile = args[1];

	return run(defFile, callFile);
}


int main(int charc, char* argv[]) {
	vector<string> args;
	for (int i = 1; i < charc; i++) {
		args.push_back(argv[i]);
	}

	mainpp(move(args));

	return 0;
}
