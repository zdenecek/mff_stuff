#include <iostream>
#include <string>

using namespace std;

class C { 
    int x;
    static int copyCount ;
public:
    C() {
        cout << 2 << endl;
        this->x = 2;
    }
    C(const C& v) {
        
        this->x = v.x + 2;
        if (C::copyCount == 2) this->x += 4;
        cout  << this->x << endl;
        C::copyCount++;
    }
    ~C() {
        if (this->x == 42)
            cout << 16 << endl;
    }
    C(C&& v) {
        cout << 10 << endl;
    }
    C& operator=(const C& v) {
        cout << 12 << endl;
        return *this;
    }
    C& operator=(C&& v) {
        cout << 14 << endl;
        this->x = 42;
        return *this;
    }
};

int C::copyCount = 0;

// Don’t touch anything below!!!
void fn_copy(C) {}
void fn_cref(const C&) {}
void fn_rref(C&&) {}
int main(int argc, char* argv[])
{
	cout << "1\n";
	C c1;
	cout << "3\n";
	C c2(c1);
	cout << "5\n";
	C c3 = c2;
	cout << "7\n";
	fn_copy(c1);
	cout << "9\n";
	fn_cref(c1);
	fn_copy(std::move(c1));
	fn_rref(std::move(c2));
	cout << "11\n";
	c3 = c2;
	cout << "13\n";
	c2 = std::move(c1);
	cout << "15\n";
}