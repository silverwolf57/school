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

class headquarter{
    private:
    string side;
    int totalLife;
    int totalCount;
    int currentIdx;
    bool stopped;
    vector<int> buildOrder;     
    map<string, int> countMap;  
    const vector<int>& initialHPs;
    public:
    headquarter(string _side,int _totalLife,vector<int> _buildOrder,const vector<int>& _initialHPs):side(_side),totalLife(_totalLife),buildOrder(_buildOrder),
    initialHPs(_initialHPs){
        stopped=false;
        totalCount=0;
        currentIdx=0;
        for(int i=0;i<5;++i){countMap[warrior_name[i]]=0;}
    }

    bool is_stopped(){return stopped;}
    
    void try_make(int hour){
        if (stopped) return;

        for (int i = 0; i < 5; ++i) {
            int warriorIdx = buildOrder[currentIdx];
            int cost = initialHPs[warriorIdx];
            string name = warrior_name[warriorIdx];

            if (totalLife >= cost) {
                totalLife -= cost;
                totalCount++;
                countMap[name]++;
                
                cout << setfill('0') << setw(3) << hour << " " << side << " " << name << " " 
                     << totalCount << " born with strength " << cost << "," << countMap[name] 
                     << " " << name << " in " << side << " headquarter" << endl;

                currentIdx = (currentIdx + 1) % 5;
                
                switch(warriorIdx){
                    case 0: { 
                        dragon d(totalCount, cost, "dragon", (double)totalLife/cost);
                        d.print_status(totalLife);
                        break;
                    }
                    case 1: { 
                        ninja n(totalCount, cost, "ninja");
                        n.print_status(totalLife);
                        break;
                    }
                    case 2: { 
                        iceman i(totalCount, cost, "iceman");
                        i.print_status(totalLife);
                        break;
                    }
                    case 3: { 
                        lion l(totalCount, cost, "lion", totalLife);
                        l.print_status(totalLife);
                        break;
                    }
                    case 4: { 
                        break;
                    }
                }
            }
            currentIdx = (currentIdx + 1) % 5;
        }
        stopped = true;
        cout << setfill('0') << setw(3) << hour << " " << side << " headquarter stops making warriors" << endl;
    }
};

int main(){
    int testCases;
    while(cin>>testCases){
    for(int i=1;i<=testCases;++i){
        int M;
        cin>>M;
       
        for(int j=0;j<5;++j){
            cin>>hps[j];
        }
        vector<int> redOrder = {2, 3, 4, 1, 0};
        vector<int> blueOrder = {3, 0, 1, 2, 4};

        headquarter red("red", M, redOrder, hps);
        headquarter blue("blue", M, blueOrder, hps);
        
        cout<<"Case:"<<i<<endl;
        int hour = 0;
        while (!red.is_stopped() || !blue.is_stopped()) {
            red.try_make(hour);
            blue.try_make(hour);
            hour++;
        }
    }
}
} 