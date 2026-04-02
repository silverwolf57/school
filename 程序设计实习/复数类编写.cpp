#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
class Complex {
private:
    double r,i;
public:
    void Print() {
        cout << r << "+" << i << "i" << endl;
    }
    Complex& operator=(const char* str){
        char* p = const_cast<char*>(str);
        char* plus = strchr(p, '+');
        if (plus) {
            r = atof(p);                                                                                                                                                                     
            i = atof(plus + 1);
        } else {
            r = atof(p);
            i = 0;
        }
        return *this;
    }
};
int main() {
    Complex a;
    a = "3+4i"; a.Print();
    a = "5+6i"; a.Print();
    return 0;
}