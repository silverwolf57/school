#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

string warrior_names[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
string weapon_names[3] = {"sword", "bomb", "arrow"};
int warrior_hps[5];

class Warrior {
protected:
    int id;
    int hp;
    string name;
public:
    Warrior(int _id, int _hp, string _name) : id(_id), hp(_hp), name(_name) {}
};


class Dragon : public Warrior {
public:
    Dragon(int _id, int _hp) : Warrior(_id, _hp, "dragon") {}
    void print_extra(int remain_life) {
        double morale = (double)remain_life / hp;
        cout << "It has a " << weapon_names[id % 3] << ",and it's morale is " 
             << fixed << setprecision(2) << morale << endl;
    }
};

class Ninja : public Warrior {
public:
    Ninja(int _id, int _hp) : Warrior(_id, _hp, "ninja") {}
    void print_extra() {
        cout << "It has a " << weapon_names[id % 3] << " and a " 
             << weapon_names[(id + 1) % 3] << endl;
    }
};

class Iceman : public Warrior {
public:
    Iceman(int _id, int _hp) : Warrior(_id, _hp, "iceman") {}
    void print_extra() {
        cout << "It has a " << weapon_names[id % 3] << endl;
    }
};

class Lion : public Warrior {
public:
    Lion(int _id, int _hp, int _loyalty) : Warrior(_id, _hp, "lion") {
        cout << "It's loyalty is " << _loyalty << endl;
    }
};

class Wolf : public Warrior {
public:
    Wolf(int _id, int _hp) : Warrior(_id, _hp, "wolf") {}
};

class Headquarter {
private:
    string side;
    int totalLife;
    int totalCount;
    int curOrderIdx;
    bool stopped;
    int order[5];      
    int counts[5];     
public:
    Headquarter(string _side, int _life, int _order[5]) 
        : side(_side), totalLife(_life), totalCount(0), curOrderIdx(0), stopped(false) {
        for(int i = 0; i < 5; ++i) {
            order[i] = _order[i];
            counts[i] = 0;
        }
    }

    bool is_stopped() { return stopped; }

    void try_make(int hour) {
        if (stopped) return;

        int searched = 0;
        while (searched < 5) {
            int typeIdx = order[curOrderIdx];
            int cost = warrior_hps[typeIdx];
            
            if (totalLife >= cost) {
                totalLife -= cost;
                totalCount++;
                counts[typeIdx]++;
                
                cout << setfill('0') << setw(3) << hour << " " << side << " " << warrior_names[typeIdx] 
                     << " " << totalCount << " born with strength " << cost << "," << counts[typeIdx] 
                     << " " << warrior_names[typeIdx] << " in " << side << " headquarter" << endl;

                if (typeIdx == 0) { 
                    Dragon d(totalCount, cost);
                    d.print_extra(totalLife);
                } else if (typeIdx == 1) { 
                    Ninja n(totalCount, cost);
                    n.print_extra();
                } else if (typeIdx == 2) { 
                    Iceman i(totalCount, cost);
                    i.print_extra();
                } else if (typeIdx == 3) { 
                    Lion l(totalCount, cost, totalLife);
                }
                
                curOrderIdx = (curOrderIdx + 1) % 5;
                return; 
            }
            curOrderIdx = (curOrderIdx + 1) % 5;
            searched++;
        }
        stopped = true;
        cout << setfill('0') << setw(3) << hour << " " << side << " headquarter stops making warriors" << endl;
    }
};

int main() {
    int testCases;
    if (!(cin >> testCases)) return 0;
    
    for (int i = 1; i <= testCases; ++i) {
        int M;
        cin >> M;
        for (int j = 0; j < 5; ++j) cin >> warrior_hps[j];
        int redOrder[5] = {2, 3, 4, 1, 0};  
        int blueOrder[5] = {3, 0, 1, 2, 4}; 

        Headquarter red("red", M, redOrder);
        Headquarter blue("blue", M, blueOrder);

        cout << "Case:" << i << endl;
        int hour = 0;
        while (!red.is_stopped() || !blue.is_stopped()) {
            red.try_make(hour);
            blue.try_make(hour);
            hour++;
        }
    }
    return 0;
}