#include <iostream>
#include <cstring>
using namespace std;

class Array2 {
      private:
      int** p;
      public:
      Array2(){}
      Array2(const int a,const int b){
           p=new int*[a];
           for(int i=0;i<a;++i){
              p[i]=new int[b];
           }
      }
      int *operator[](int n){
            return p[n];
      }
      int &operator()(const int n,const int m){
           return p[n][m];
      }
};

int main() {
    Array2 a(3,4);
    int i,j;
    for(  i = 0;i < 3; ++i )
        for(  j = 0; j < 4; j ++ )
            a[i][j] = i * 4 + j;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << a(i,j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;     b = a;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}