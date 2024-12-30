#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

extern vector<pair<string, function<void()>>> tests;

void expect_failed(const string& message) {
    cerr << "Test error: " << message << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    vector<string> required_tests;

    if (argc > 1) {
        required_tests.assign(argv + 1, argv + argc);
    } else {
        for (const auto& test : tests)
            required_tests.push_back(test.first);
    }

    for (const auto& required_test : required_tests) {
        bool found = false;
        for (const auto& test : tests)
            if (required_test == test.first) {
                cerr << "Running test " << required_test << endl;
                test.second();
                found = true;
                break;
            }
        if (!found) {
            cerr << "Unknown test " << required_test << endl;
            return 1;
        }
    }

    return 0;
}
