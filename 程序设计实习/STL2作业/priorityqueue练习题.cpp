/*
描述
我们定义一个正整数a比正整数b优先的含义是：
*a的质因数数目（不包括自身）比b的质因数数目多；
*当两者质因数数目相等时，数值较大者优先级高。


现在给定一个容器，初始元素数目为0，之后每次往里面添加10个元素，每次添加之后，
要求输出优先级最高与最低的元素，并把该两元素从容器中删除。

输入
第一行: num (添加元素次数，num <= 30)

下面10*num行，每行一个正整数n（n < 10000000).

输出
每次输入10个整数后，输出容器中优先级最高与最低的元素，两者用空格间隔。

样例输入
1
10 7 66 4 5 30 91 100 8 9
样例输出
66 5
*/

#include <iostream>
#include <cmath>
#include <set>
using namespace std;

bool check(int a){
    for (int i = 2; i <= sqrt(a); i++){
        if (a%i == 0)
            return false;
    }
    return true;
}

int getFactor(int a){
    int num = 0;
    int k;
    for (int i = 2; i <= sqrt(a); i++){ 
        if (a%i == 0){
            k = a / i;
            if (i != k && check(k)) 
                num++;
            if (check(i)) 
                num++;
        }
    }
    return num;
}

class Compare{
public:
    bool operator()(const int& a1, const int& a2)const{
        int num1 = 0, num2 = 0;
        num1 = getFactor(a1);
        num2 = getFactor(a2);
        if (num1 < num2)
            return true;
        else if (num1 > num2)
            return false;
        else if (num1 == num2)
            return(a1 < a2);
    }
};

int main(){
    set<int, Compare> pqueue;
    int n, num;
    cin >> n;
    for(int i=0;i<n;++i){
        for (int j = 0; j < 10; j++){
            cin >> num;
            pqueue.insert(num);
        }
        int min = *(pqueue.begin());
        int max = *(pqueue.rbegin());
        pqueue.erase(min);
        pqueue.erase(max);
        cout << max << ' ' << min << endl;
    }
    return 0;
}