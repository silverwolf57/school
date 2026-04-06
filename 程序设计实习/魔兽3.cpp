#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<iomanip>

using namespace std;

string warrior_name[5]={"dragon","ninja","iceman","lion","wolf"};
string weapon_name[3]={"sword","bomb","arrow"};
vector<int> hps(5);

class warrior{
    protected:
    int id;
    int hp;
    string name;
    public:
    warrior(){}
    warrior(int _id,int _hp,string _name):id(_id),hp(_hp),name(_name){}
    virtual ~warrior(){}
    virtual void print_status(int remain) = 0;

};

class dragon:public warrior{
      private:
      string weapon;
      double morale;
      public:
      dragon(int _id,int _hp,string _name,double _morale):warrior(_id,_hp,"dragon"),morale(_morale){}
      void print_status(int remain)override {
        weapon=weapon_name[id%3];
        morale=(double)remain/hp;
        cout<<"It has a "<<weapon<<",and it's morale is "<<fixed<<setprecision(2)<<morale<<endl;
      }
};

class ninja:public warrior{
    private:
    string weapon1,weapon2;
    public:
    ninja(int _id,int _hp,string _name):warrior(_id,_hp,"ninja"){}
    void print_status(int remain)override{
         weapon1=weapon_name[id%3];
         weapon2=weapon_name[(id+1)%3];
         cout<<"It has a "<<weapon1<<" and a "<<weapon2<<endl;
    }
};

class iceman:public warrior{
    private:
    string weapon;
    public:
    iceman(int _id,int _hp,string _name):warrior(_id,_hp,"iceman"){}
    void print_status(int remain)override{
        weapon=weapon_name[id%3];
        cout<<"It has a "<<weapon<<endl;
    }
};

class lion:public warrior{
    private:
    int loyalty;
    public:
    lion(int _id,int _hp,string _name,int _loyalty):warrior(_id,_hp,"lion"),loyalty(_loyalty){}
    void print_status(int remain)override{
        cout<<"It's loyalty is "<<loyalty<<endl;
    }
};

class wolf:public warrior{
    public:
    wolf(int _id,int _hp,string _name):warrior(_id,_hp,"wolf"){}
    void print_status(int remain) override {}
};
