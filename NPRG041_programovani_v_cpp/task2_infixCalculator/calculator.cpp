#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <stack>
#include <variant>
#include <memory>
#include <vector>

using namespace std;

using Value = std::variant<int, double>;

struct ToStringVisitor
{
	std::string operator()(int value) const
	{
		return std::to_string(value);
	}

	std::string operator()(double value) const
	{
		// Remove unnecessary digits after decimal point
		std::ostringstream oss;
		oss << std::noshowpoint << value;
		return oss.str();
	}
};

struct ValueVisitor
{
	int operator()(int value) const
	{
		return value;
	}

	double operator()(double value) const
	{
		return value;
	}
};

constexpr char variable_indicator = '$';

// Helper function to check if a character is a digit
bool is_digit(char c)
{
	return isdigit(c) || c == '.';
}

// Helper function to check if a character is an operator
bool is_operator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

// Helper function to check if a character is a parenthesis
bool is_parenthesis(char c)
{
	return c == '(' || c == ')';
}

// Helper function to check if a character is a variable name character
bool is_variable_char(char c)
{
	return isalpha(c);
}

bool is_int(std::string const &str)
{
	int result;
	auto i = istringstream(str);

	i >> result;

	return !i.fail() && i.eof();
}

// Enum to represent the type of a token (number, operator, parenthesis, variable)
enum class TokenType
{
	NUMBER,
	OPERATOR,
	PARENTHESIS,
	VARIABLE,
};

enum class ParenType
{
	LEFT,
	RIGHT
};

class Token
{
public:
	virtual TokenType get_type() const = 0;
	virtual ~Token() = default;
};

template <typename T>
class NumberToken : public Token
{
private:
	T value;

public:
	NumberToken(T value) : value(value) {}
	TokenType get_type() const override
	{
		return TokenType::NUMBER;
	}
	T get_value() const
	{
		return value;
	}
	
};

class VariableToken : public Token
{
private:
	string name;

public:
	Value get_value(const unordered_map<string, variant<int, double>> &context) const
	{

		try {
			
		return context.at(name);
		}
		catch (...) {
			throw runtime_error("Use of unassigned variable");
		}
	}

	shared_ptr<Token> get_value_as_token(const unordered_map<string, variant<int, double>> &context) const
	{
		auto value = get_value(context);

		if (holds_alternative<int>(value))
		{
			return make_shared<NumberToken<int>>(NumberToken<int>(get<int>(value)));
		}
		else
		{
			return make_shared<NumberToken<double>>(NumberToken<double>(get<double>(value)));
		}
	}

	TokenType get_type() const override
	{
		return TokenType::VARIABLE;
	}
	VariableToken(string name) : name(name) {}
};

class OperatorToken : public Token
{

private:
	char value;

public:
	OperatorToken(char value) : value(value) {}

	TokenType get_type() const override
	{
		return TokenType::OPERATOR;
	}

	char get_value() const
	{
		return value;
	}
};

class ParethesisToken : public Token
{
private:
	ParenType type;

public:
	ParethesisToken(ParenType type) : type(type) {}

	TokenType get_type() const override
	{
		return TokenType::PARENTHESIS;
	}

	ParenType get_paren_type() const
	{
		return type;
	}
};

bool is_paren(shared_ptr<Token> token, ParenType type)
{
	return token->get_type() == TokenType::PARENTHESIS && (dynamic_pointer_cast<ParethesisToken>(token))->get_paren_type() == type;
}

// Class to implement the calculator
class Calculator
{
private:
	// Map to store the variables and their values
	unordered_map<string, Value> variables;
	int line_counter = 0;

	// Helper function to get the priority of an operator
	int get_priority(char op)
	{
		if (op == '+' || op == '-')
		{
			return 1;
		}
		else if (op == '*' || op == '/' || op == '%')
		{
			return 2;
		}
		return 0;
	}

	template <typename T>
	T perform_operation(T num1, char op, T num2)
	{
		if (op == '+')
		{
			return num1 + num2;
		}
		else if (op == '-')
		{
			return num1 - num2;
		}
		else if (op == '*')
		{
			return num1 * num2;
		}
		else if (op == '/')
		{
			if (num2 == 0 || num2 == -0)
				throw runtime_error("Division by zero");
			return num1 / num2;
		}
		else if (op == '%')
		{
			if (num2 == 0 || num2 == -0)
				throw runtime_error("Division by zero");
			return fmod(num1, num2);
		}
		return 0;
	}

	// Helper function to perform an operation on two numbers
	Value perform_operation(Value num1, char op, Value num2)
	{
		if (holds_alternative<double>(num1) || holds_alternative<double>(num2))
		{
			double val1 = holds_alternative<double>(num1) ? get<double>(num1) : get<int>(num1);
			double val2 = holds_alternative<double>(num2) ? get<double>(num2) : get<int>(num2);
			return perform_operation(val1, op, val2);
		}
		else
		{
			return perform_operation(get<int>(num1), op, get<int>(num2));
		}
	}

	// Helper function to evaluate an expression
	Value evaluate_expression(string expr)
	{
		// Convert the tokens to reverse polish notation (RPN) using the Shunting-yard algorithm
		
		vector<shared_ptr<Token>> rpn = to_rpn(tokenize(expr));
		

		// Evaluate the RPN expression
		return evaluate_rpn(move(rpn));
	}

	shared_ptr<Token> parse_number_or_variable(string &value)
	{
		// Check if the value is a number or a variable
		if (isdigit(value[0]))
		{
			if (is_int(value))
				return make_shared<NumberToken<int>>(NumberToken<int>(stoi(value)));
			else
				return make_shared<NumberToken<double>>(NumberToken<double>(stod(value)));
		}
		else
		{
			if (value[0] != variable_indicator)
				throw runtime_error("Invalid variable name");

			// Convert the variable name to lowercase
			transform(value.begin(), value.end(), value.begin(), ::tolower);
			return make_shared<VariableToken>(VariableToken(value.substr(1)));
		}
	}

	// Helper function to split an expression into tokens

	vector<shared_ptr<Token>> tokenize(const string& expr)
	{
		// we have to check for emtpy parentheses explicitly
		if(expr.find("()") != string::npos)
			throw runtime_error("Invalid expression");

		vector<shared_ptr<Token>> tokens;
		string current_token;

		bool operand_expected = true;
		bool negative = false;

		for (char c : expr)
		{
			if (is_digit(c) || is_variable_char(c) || c == variable_indicator)
			{
				current_token += c;
			}
			else if (is_operator(c) || is_parenthesis(c))
			{

				if (!current_token.empty())
				{
					tokens.push_back(move(parse_number_or_variable(current_token)));
					current_token.clear();
					operand_expected = false;
					negative = false;
				}

				if (c == '-' && operand_expected && !negative)
				{
					tokens.push_back(make_shared<NumberToken<int>>(NumberToken<int>(-1)));
					tokens.push_back(move(make_shared<OperatorToken>(OperatorToken('*'))));
					negative = true;
					continue;
				}

				if(is_parenthesis(c))
					tokens.push_back(make_shared<ParethesisToken>(ParethesisToken(c == '(' ? ParenType::LEFT : ParenType::RIGHT)));
				else 

				tokens.push_back(make_shared<OperatorToken>(OperatorToken(c)));
				if (!is_parenthesis(c))
					operand_expected = true;
				else
					negative = false;
			}
			else if (isspace(c))
			{
				if (!current_token.empty())
				{
					tokens.push_back(move(parse_number_or_variable(current_token)));
					current_token.clear();
					operand_expected = false;
					negative = false;
				}
			}
			else
			{
				// Invalid character
				throw runtime_error("Invalid character in expression: " + string(1, c));
			}
		}
		if (!current_token.empty())
		{
			tokens.push_back(move(parse_number_or_variable(current_token)));
		}
		return tokens;
	}

	// Helper function to convert a list of tokens to reverse polish notation (RPN) using the Shunting-yard algorithm
	vector<shared_ptr<Token>> to_rpn(vector<shared_ptr<Token>> &&tokens)
	{
		vector<shared_ptr<Token>> rpn;
		stack<shared_ptr<Token>> stack;

		reverse(tokens.begin(), tokens.end());
		while (tokens.size() > 0)
		{
			shared_ptr<Token> token = std::move(tokens.back());
			tokens.pop_back();

			if (token->get_type() == TokenType::NUMBER)
			{
				rpn.push_back(token);
			}
			else if (token->get_type() == TokenType::VARIABLE)
			{
				shared_ptr<VariableToken> var = dynamic_pointer_cast<VariableToken>(token);

				// Replace variable with its value
				rpn.push_back(var->get_value_as_token(variables));
			}
			else if (token->get_type() == TokenType::PARENTHESIS)
			{
				if (dynamic_pointer_cast<ParethesisToken>(token)->get_paren_type() == ParenType::LEFT)
				{
					stack.push(token);
				}
				else
				{
					while (!stack.empty() && !is_paren(stack.top(), ParenType::LEFT))
					{
						rpn.push_back(stack.top());
						stack.pop();
					}
					if (stack.empty() || !is_paren(stack.top(), ParenType::LEFT))
					{
						throw runtime_error("Mismatched parenthesis in expression");
					}
					stack.pop();
				}
			}
			else if (token->get_type() == TokenType::OPERATOR)
			{
				char op = dynamic_pointer_cast<OperatorToken>(token)->get_value();

				int priority = get_priority(op);
				while (!stack.empty() && stack.top()->get_type() == TokenType::OPERATOR && get_priority(dynamic_pointer_cast<OperatorToken>(stack.top())->get_value()) >= priority)
				{
					rpn.push_back(stack.top());
					stack.pop();
				}
				stack.push(token);
			}
		}
		while (!stack.empty())
		{
			if (stack.top()->get_type() == TokenType::PARENTHESIS)
			{
				throw runtime_error("Mismatched parenthesis in expression");
			}
			rpn.push_back(stack.top());
			stack.pop();
		}
		return rpn;
	}

	// Helper function to evaluate a reverse polish notation (RPN) expression
	Value evaluate_rpn(vector<shared_ptr<Token>> &&rpn)
	{
		stack<Value> stack;
		for (shared_ptr<Token> token : rpn)
		{
			if (token->get_type() == TokenType::NUMBER)
			{
				if (dynamic_pointer_cast<NumberToken<int>>(token))
				{
					shared_ptr<NumberToken<int>> p = dynamic_pointer_cast<NumberToken<int>>(token);
					stack.push(p->get_value());
				}
				else {
					shared_ptr<NumberToken<double>> p = dynamic_pointer_cast<NumberToken<double>>(token);
					stack.push(p->get_value());
				}
			}
			else if (token->get_type() == TokenType::OPERATOR)
			{
				if (stack.size() < 2)
				{
					throw runtime_error("Invalid expression");
				}
				Value num2 = stack.top();
				stack.pop();
				Value num1 = stack.top();
				stack.pop();
				char op = dynamic_pointer_cast<OperatorToken>(token)->get_value();
				Value result = perform_operation(num1, op, num2);
				stack.push(result);
			}
		}
		if (stack.size() != 1)
		{
			throw runtime_error("Invalid expression");
		}
		return stack.top();
	}

public:
	// Function to evaluate a line of input
	void evaluate(string line)
	{
		line_counter++;
		// Check if the line is a variable assignment
		size_t pos = line.find('=');
		if (pos != string::npos)
		{
			// Parse the variable name and expression
			string variable_name = line.substr(0, pos);

			// Convert the variable name to lowercase
			transform(variable_name.begin(), variable_name.end(), variable_name.begin(), ::tolower);
			string expression = line.substr(pos + 1);

			// Check if the variable name is valid
			for (char c : variable_name)
			{
				if (!is_variable_char(c))
				{
					cerr << "line " << line_counter << ": Error: invalid variable name " << variable_name << ", symbol " << c << endl;
					cout << "invalid" << endl;
					return;
				}
			}

			if (variables.find(variable_name) != variables.end())
			{
				cerr << "line " << line_counter << ": Error: cannot reassign variable: " << variable_name << endl;
				cout << "invalid" << endl;
				return;
			}

			// Trim the variable name and expression
			expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
			// Evaluate the expression
			try
			{
				Value value = evaluate_expression(expression);
				// Assign the value to the variable
				variables[variable_name] = value;
				cout << std::noshowpoint << visit(ToStringVisitor{}, value) << endl;
			}
			catch (exception &e)
			{
				cerr << "line " << line_counter << ": Error: " << e.what() << endl;
				cout << "invalid" << endl;
			}
		}
		else
		{
			// The line is an expression, evaluate it
			try
			{
				Value value = evaluate_expression(line);
				cout << visit(ToStringVisitor{}, value) << endl;
			}
			catch (exception &e)
			{
				cerr << "line " << line_counter << ": Error: " << e.what() << endl;
				cout << "invalid" << endl;
			}
		}
	}
};

int main()
{

	Calculator calculator;
	string line;

	int n = 0;
	getline(cin, line);

	try
	{
		n = stoi(line);
	}
	catch (...)
	{
		cout << "Invalid number of expressions" << endl;
	}

	for (int i = 0; i < n; i++)
	{
		getline(cin, line);

		calculator.evaluate(line);
	}

	return 0;
}