
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * @brief tries to parse int at num_string and add or subtract it to sum
 * @param num_str string to try to parse
 * @param sum sum to add or subtract to
 * @return 0 if success, 1 if error
 */
int resolve_operand(int &sum, ostringstream &num_str, bool plus)
{
    int operand;
    try
    {
        operand = stoi(num_str.str());
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Invalid syntax" << endl;
        return 1;
    }
    catch (const out_of_range &e)
    {
        std::cout << "Out of range" << endl;
        return 1;
    }
    num_str.str("");
    sum += plus ? operand : -operand;

    return 0;
}

int evaluate_string(string &str)
{
    int sum = 0;

    ostringstream num;
    bool plus = true;
    bool first_character = true;

    for (auto &ch : str)
    {
        if (ch == '+' || ch == '-')
        {
            if (!first_character)
            {
                int operation_result = resolve_operand(sum, num, plus);
                if (operation_result != 0)
                    return 1;
            }
            plus = ch == '+';
        }
        else if (isdigit(ch))
        {
            num << ch;
        }
        else
        {
            std::cout << "Invalid character: " << ch << endl;
            return 1;
        }

        first_character = false;
    }

    int operation_result = resolve_operand(sum, num, plus);
    if (operation_result != 0)
        return 1;

    std::cout << sum << endl;
    return 0;
}

int _main(vector<string> args)
{
    if (args.size() < 2)
    {
        std::cout << "Please provide an expression" << endl;
        return 1;
    }

    int argsc = args.size();
    for(int i = 1; i < argsc; i++)
    {
        evaluate_string(args[i]);
    }

    return 0;
}

// TODO add option strict to terminate on error.

int main(int argc, char **argv)
{
    vector<string> args(argv, argv + argc);
    return _main(args);
}
