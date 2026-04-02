#include <iostream> 
#include <cstring> 
#include <cstdlib> 
#include <cstdio> 
#include <algorithm>
using namespace std;

const int MAX = 210; 

class CHugeInt {
private:
    char* c;
    char* addStrings(const char* s1, const char* s2) const {
        int len1 = strlen(s1);
        int len2 = strlen(s2);
        int maxLen = max(len1, len2);
        int* temp = new int[maxLen + 1](); 
        for (int i = 0; i < len1; ++i) temp[i] += s1[len1 - 1 - i] - '0';
        for (int i = 0; i < len2; ++i) temp[i] += s2[len2 - 1 - i] - '0';
        int realLen = maxLen;
        for (int i = 0; i < realLen; ++i) {
            if (temp[i] >= 10) {
                temp[i+1] += temp[i] / 10;
                temp[i] %= 10;
                if (i == realLen - 1) realLen++; 
            }
        }
        char* res = new char[realLen + 1];
        for (int i = 0; i < realLen; ++i) {
            res[i] = temp[realLen - 1 - i] + '0';
        }
        res[realLen] = '\0';
        delete[] temp;
        return res;
    }

public:
    CHugeInt() {}
    CHugeInt(int n){
        char buffer[MAX];
        sprintf(buffer, "%d", n);
        c = new char[strlen(buffer) + 1];
        strcpy(c, buffer);
    }
    CHugeInt(const char* s) {
        c = new char[strlen(s) + 1];
        strcpy(c, s);
    }
    CHugeInt(const CHugeInt& s) {
        c = new char[strlen(s.c) + 1];
        strcpy(c, s.c);
    }
    CHugeInt& operator=(const CHugeInt& s) {
        if (this != &s) {
            delete[] c;
            c = new char[strlen(s.c) + 1];
            strcpy(c, s.c);
        }
        return *this;
    }
    ~CHugeInt() { delete[] c; }
    CHugeInt operator+(const CHugeInt& s) const {
        char* resStr = addStrings(this->c, s.c);
        CHugeInt res(resStr);
        delete[] resStr;
        return res;
    }
    CHugeInt operator+(int n) const {
        return *this + CHugeInt(n);
    }
    friend CHugeInt operator+(int n, const CHugeInt& s) {
        return CHugeInt(n) + s;
    }
    CHugeInt& operator+=(int n) {
        *this = *this + n;
        return *this;
    }
    CHugeInt& operator++() {
        *this += 1;
        return *this;
    }
    CHugeInt operator++(int) {
        CHugeInt temp(*this);
        *this += 1;
        return temp;
    }
    friend ostream& operator<<(ostream& o, const CHugeInt& S) {
        o << S.c;
        return o;
    }
};

int main() 
{ 
  char s[210];
  int n;

  while (cin >> s >> n) {
    CHugeInt a(s);
    CHugeInt b(n);

    cout << a + b << endl;
    cout << n + a << endl;
    cout << a + n << endl;
    b += n;
    cout  << ++ b << endl;
    cout << b++ << endl;
    cout << b << endl;
  }
  return 0;
}