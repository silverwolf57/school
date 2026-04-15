/*
输入
多组数据。每组第一行是一个不含空格的字符串
第二行是整数n
第三行是n个整数
输出
对每组数据，先依次输出输入字符串的每个字母，并且在每个字母后面加逗号
然后依次再输出输入的n个整数 ，在每个整数后面加逗号
样例输入
Tom 
3
3 4 5
Jack
4
1 2 3 4
样例输出
T,o,m,
3,4,5,
J,a,c,k,
1,2,3,4,
*/
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
template <class T>  
class myclass {
// 在此处补充你的代码
private:
T* p;
int size;
public:
myclass(T *q,int num):size(num){
    p=new T[size];
    for(int i=0;i<size;++i){
        p[i]=q[i];
    }
}
~myclass( ) {
		delete [] p;
	}
	void Show()
	{
		for( int i = 0;i < size;i ++ ) {
			cout << p[i] << ",";
		}
		cout << endl;
	}
};
int a[100];
int main() {
	char line[100];
	while( cin >> line ) {
		myclass<char> obj(line,strlen(line));;
		obj.Show();
		int n;
		cin >> n;
		for(int i = 0;i < n; ++i)
			cin >> a[i];
		myclass<int> obj2(a,n);
		obj2.Show();
	}
	return 0;
}
/*
对于本题，除了注意一下写构造函数时指针要初始化，没什么可说的
*/