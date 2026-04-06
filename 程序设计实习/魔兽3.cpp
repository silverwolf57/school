#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<iomanip>
#include<algorithm>

using namespace std;

string warrior_name[5]={"dragon","ninja","iceman","lion","wolf"};
string weapon_name[3]={"sword","bomb","arrow"};
vector<int> hps(5);
class weapon{
    friend class warrior;
    private:
    int type;
    int attack_force;
    int remain_uses;//designed for the arrows
    public:
    weapon(int _type,int _attack_force,int _used):type(_type),attack_force(_attack_force){}
    void restore_uses(){
        if(type==2){
            remain_uses=2;
        }else if (type==1){
            remain_uses=1;
        }else{
            remain_uses=9999;
        }
    }
    int get_damage(int force){
        if(type==0){
           return force*2/10;
        }  
        else if(type==1){
            return force*4/10;
        }
        else{
            return force*3/10;
        }
    }//caculate the damage according to the weapon
    int self_damage(int attack_damage){
        if(type==1)return attack_damage/2;
        return 0;
    }//damage caculation designed for the bomb
    ~weapon(){}
};
class warrior{
    protected:
    int id;
    int hp;
    string name;
    int force;
    int color;
    int position;
    vector<weapon> weapons;
    int loyalty;//used for lions
    int steps;//used for the iceman
    public:
    warrior(int _id,int _hp,string _name,int _color):id(_id),hp(_hp),name(_name),
    color(_color),steps(0){}
    ~warrior(){}
    void sort_weapons(){
         if (weapons.empty())return;
         
         sort(weapons.begin(),weapons.end(),[](const weapon& a,const weapon& b){
             if(a.type!=b.type){
                return a.type<b.type;
             }
             else if(a.type==2&&b.type==2){
                return a.remain_uses<b.remain_uses;
             }
             return false;
         });
    }//sort the weapons
    virtual void add_weapon(weapon w){
        if (weapons.size() < 10) {
        weapons.push_back(w);
      }
    }
    void rob_weapons(){}//designed for the wolves
    virtual void march(){
        if(color==0){
            position++;
        }else{
            position--;
        }
    }//move the warrior
    virtual void attack(warrior& w,weapon &wpn){
        w.hp-=wpn.get_damage(force);
        if(wpn.type==1)hp-=wpn.self_damage();
        else if(wpn.type==2)wpn.remain_uses--;
    }//attack
    bool is_dead(){
        if (hp<0)return true;
    }//check the state
    void should_escape(){}//for the lion to run away
};

class dragon:public warrior{
    public:
    warrior(int _id,int _hp,string _name,int _color):id(_id),hp(_hp),name(_name),
    color(_color),steps(0){}
};
