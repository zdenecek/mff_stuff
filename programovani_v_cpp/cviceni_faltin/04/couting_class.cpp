#include <iostream>
#include <vector>

using namespace std;

class CountingClass {
    static size_t num_instances;
    static void inc_num_instances() { ++num_instances; }
    static void dec_num_instances() { --num_instances; }

public:
    static bool has_instance() { return num_instances > 0; }
    static size_t get_num_instances() { return num_instances; }
    
    CountingClass() { inc_num_instances(); }
    CountingClass(const CountingClass &) { inc_num_instances(); }
    ~CountingClass() { dec_num_instances(); }
};  

size_t CountingClass::num_instances = 0; // initialization

void f() {
    cout << CountingClass::get_num_instances() << endl; // 0
    CountingClass cc1;
    cout << CountingClass::get_num_instances() << endl; // 1
    CountingClass cc2 = cc1;
    cout << CountingClass::get_num_instances() << endl; // 2
    
    std::vector<CountingClass> ccs(10);
    cout << CountingClass::get_num_instances() << endl; // 12
}

int main() {
    cout << CountingClass::get_num_instances() << endl; // 0
    f();
    cout << CountingClass::get_num_instances() << endl; // 0
}

