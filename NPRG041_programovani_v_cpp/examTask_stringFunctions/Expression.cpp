#include "Expression.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

#include "Function.h"


using namespace std;
using FunctionTable = unordered_map<string, Function>;

string FunctionalExpression::evaluate(FunctionTable context) const {


	if (context.find(functionName) == context.end()) {
		throw runtime_error("nodef error");
	}

	Function& fun = context.at(functionName);

	if (fun.argc != arguments.size()) {
		throw runtime_error("param error");
	}

	stringstream ss;

	bool first = true;

	for (int unit : fun.translations) {
		if (!first) ss << ' ';
		ss << arguments[unit]->evaluate(context);
		first = false;
	}

	return ss.str();
}


string ConstantExpression::evaluate(FunctionTable context) const {
	return value;
}
