#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <cstdlib>
using namespace std;

class Student {  
    private:
       std::string receive,name;
       int age,num,grd1,grd2,grd3,grd4;
       double average;
    public:
       void input(){
            getline(std::cin,receive);
            std::stringstream ss(receive);
            getline(ss,name,',');
            std::string temp;
            getline(ss,temp,',');
            age=stoi(temp);
            getline(ss,temp,',');
            num=stoi(temp);
            getline(ss,temp,',');
            grd1=stoi(temp);
            getline(ss,temp,',');
            grd2=stoi(temp);
            getline(ss,temp,',');
            grd3=stoi(temp);
            getline(ss,temp,',');
            grd4=stoi(temp);
       }
       void calculate(){
            average=static_cast<double>((grd1+grd2+grd3+grd4)/4.0);
       }
       void output(){
           std::cout<<name<<','<<age<<','<<num<<','<<average;
       }     
};

int main() {
	Student student;        // 定义类的对象
	student.input();        // 输入数据
	student.calculate();    // 计算平均成绩
	student.output();       // 输出数据
}