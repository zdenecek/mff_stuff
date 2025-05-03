#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


#include "Function.h"


using namespace std;
using FunctionTable = unordered_map<string, Function>;


class Expression {
public:
	virtual string evaluate(FunctionTable context) const = 0;

};

class ConstantExpression : public Expression {
	string value;
public:
	ConstantExpression(string value) : value(value) {}
	string evaluate(FunctionTable context) const override;
};

class FunctionalExpression : public Expression {

public:
	string functionName;
	vector<unique_ptr<Expression>> arguments;


	FunctionalExpression(string functionName) : functionName(functionName) {

	}

	string evaluate(FunctionTable context) const override;

};