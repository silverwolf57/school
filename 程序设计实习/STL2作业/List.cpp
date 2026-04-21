/*
描述
写一个程序完成以下命令：
new id ——新建一个指定编号为id的序列(id < 10000)
add id num——向编号为id的序列加入整数
num merge id1 id2——如果id1等于id2,不做任何事，否则归并序列id1和id2中的数，并将id2清空
unique id——去掉序列id中重复的元素
out id ——从小到大输出编号为id的序列中的元素，以空格隔开

输入
第一行一个数n，表示有多少个命令( n＜＝２０００００)。以后n行每行一个命令。
输出
按题目要求输出。
样例输入
16
new 1
new 2
add 1 1
add 1 2
add 1 3
add 2 1
add 2 2
add 2 3
add 2 4
out 1
out 2
merge 1 2
out 1
out 2
unique 1
out 1
样例输出
1 2 3 
1 2 3 4
1 1 2 2 3 3 4

1 2 3 4*/
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;

int main(){
    int n;
    cin>>n;
    map<int,vector<int>> store;
    for(int i=0;i<n;++i){
        string s;
        cin>>s;
        if(s=="new"){
            int l;
            std::cin>>l;
            vector<int> num;
            store[l]=num;
        }
        else if(s=="add"){
            int l,k;
            std::cin>>l>>k;
            store[l].push_back(k);
        }
        else if(s=="out"){
            int l;
            std::cin>>l;
            sort(store[l].begin(),store[l].end());
            cout<<store[l][0];
            for(int i=1;i<store[l].size();++i){
                cout<<" "<<i;
            }
        }
        else if(s=="unique"){
            
        }
    }
}