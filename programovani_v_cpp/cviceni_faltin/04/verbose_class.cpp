#include <iostream>

using namespace std;

class Verbose {
    int x;
public:
    Verbose() { 
        cout << "default ctor\n";
        this->x = 1;
    }
    Verbose(const Verbose &v) { 
        cout << "copy ctor\n"; 
        this->x = v.x;        
    }
    Verbose(Verbose &&v) { 
        cout << "move ctor\n"; 
        this->x = v.x;
        v.x = 0;
    }
    ~Verbose() { 
        cout << "dtor\n"; 
    }
    Verbose(int x) { 
        cout << "user ctor\n";
        this->x = x;  
    }
    Verbose &operator=(const Verbose &v) { 
        cout << "copy assignment\n"; 
        this->x = v.x;
        return *this; 
    }
    Verbose &operator=(Verbose &&v) { 
        cout << "move assignment\n"; 
        this->x = v.x;
        return *this; 
    }
};

int main()
{
    Verbose v1; // default ctor
    Verbose v2(2); // user ctor
    Verbose v3{3}; // user  ctor
    Verbose v4(v2); // copy ctor
    Verbose v5 = v3; // copy ctor
    Verbose v6(std::move(v1)); // move ctor
    Verbose v7 = std::move(v4); // move ctor
    v1 = v2; // copy assignment
    v2 = std::move(v3); // move assignment
} // Calls destructors
