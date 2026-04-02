#include <iostream>
using namespace std;
class B { 
	private: 
		int nBVal; 
	public: 
		void Print() 
		{ cout << "nBVal="<< nBVal << endl; } 
		void Fun() 
		{cout << "B::Fun" << endl; } 
		B ( int n ) { nBVal = n;} 
};
class D:public B{
    private:
    int nDval;
    public:
    D(int n):B(3*n),nDval(n){}
    void Print(){
		B::Print();
        cout<<"nDval="<<nDval<<endl;
    }
    void Fun(){
        cout<<"D::fun"<<endl;
    }
};
int main() { 
	B * pb; D * pd; 
	D d(4); d.Fun(); 
	pb = new B(2); pd = new D(8); 
	pb -> Fun(); pd->Fun(); 
	pb->Print (); pd->Print (); 
	pb = & d; pb->Fun(); 
	pb->Print(); 
	return 0;
}