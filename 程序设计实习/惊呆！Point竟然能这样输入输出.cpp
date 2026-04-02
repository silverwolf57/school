#include <iostream> 
using namespace std;
class Point { 
	private: 
		int x; 
		int y; 
	public: 
		Point() { };
        friend ostream & operator<<(ostream& o,const Point &p){
              o<<p.x<<','<<p.y;
              return o;
        }
        friend istream & operator>>(istream& is,Point &p){
            is>>p.x>>p.y;
            return is;
        }
}; 
int main() 
{ 
 	Point p;
 	while(cin >> p) {
 		cout << p << endl;
	 }
	return 0;
}