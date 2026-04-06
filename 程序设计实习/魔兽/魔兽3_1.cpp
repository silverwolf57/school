#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;

// 全局状态和参数
int current_time = 0;
bool red_conquered = false;
bool blue_conquered = false;
int loyaltyDecrease, timeLimit, cityCount;

unordered_map<string, int> initialHPs;
unordered_map<string, int> initialForces;
unordered_map<string, vector<string>> buildOrder;
vector<string> weapon_kinds = {"sword", "bomb", "arrow"};

enum fight_result { BOTH_DEAD, BOTH_ALIVE, BLUE_WIN, RED_WIN };

// 获取格式化时间戳
string get_format_clock() {
    int hour = current_time / 60;
    int minute = current_time % 60;
    ostringstream ost;
    ost << setw(3) << setfill('0') << hour << ":" << setw(2) << setfill('0') << minute;
    return move(ost.str());
}

// ---------------- 武器系统解耦 ----------------
class weapon {
public:
    string name;
    weapon(string name_) : name(name_) {}
    virtual ~weapon() {}
    // 解耦：直接传入必要的战斗数值，避免依赖 warrior 类的定义
    virtual void attack(int attacker_force, string attacker_name, int& attacker_hp, int& injure_hp) {}
    virtual bool is_usable() { return false; }
};

class sword : public weapon {
public:
    sword(string name_) : weapon(name_) {}
    void attack(int attacker_force, string attacker_name, int& attacker_hp, int& injure_hp) override {
        int attack_dmg = (attacker_force * 2) / 10;
        injure_hp -= attack_dmg;
    }
    bool is_usable() override { return true; }
};

class bomb : public weapon {
public:
    bool usable;
    bomb(string name_) : weapon(name_), usable(true) {}
    void attack(int attacker_force, string attacker_name, int& attacker_hp, int& injure_hp) override {
        if (!usable) return;
        int attack_dmg = (attacker_force * 4) / 10;
        int self_attack = (attacker_name == "ninja") ? 0 : attack_dmg / 2;
        attacker_hp -= self_attack;
        injure_hp -= attack_dmg;
        usable = false;
    }
    bool is_usable() override { return usable; }
};

class arrow : public weapon {
public:
    int use_count;
    arrow(string name_, int use_count_) : weapon(name_), use_count(use_count_) {}
    void attack(int attacker_force, string attacker_name, int& attacker_hp, int& injure_hp) override {
        if (use_count >= 2) return;
        int attack_dmg = (attacker_force * 3) / 10;
        injure_hp -= attack_dmg;
        use_count++;
    }
    bool is_usable() override { return use_count < 2; }
};

// 全局宝剑共享实例
sword* sw = nullptr;

// ---------------- 战士系统 ----------------
class warrior {
public:
    int id;              // 战士编号
    int hp;              // 生命值
    int force;           // 攻击力
    string side;         // 阵营
    string name;         // 名称
    int location;        // 所在城市
    int cur_weapon_ind;  // 当前使用武器的序号
    vector<weapon*> weapons;

    warrior(int id_, int hp_, string side_, string name_, int location_, int force_)
        : id(id_), hp(hp_), side(side_), name(name_), location(location_), force(force_) {
        weapons = vector<weapon*>(10, nullptr);
    }
    
    warrior(warrior& w) {
        id = w.id;
        hp = w.hp;
        force = w.force;
        side = w.side;
        name = w.name;
        location = w.location;
        weapons = std::move(w.weapons);
    }
    
    virtual ~warrior() {}

    virtual void march() {
        int direction = side == "red" ? 1 : -1;
        location += direction;
        cout << get_format_clock() << " " << side << " " << name << " " << id 
             << " marched to city " << location << " with " << hp 
             << " elements and force " << force << endl;
    }

    void reach_headquarter() {
        if (name == "iceman") {
            int decrease_elements = hp / 10;
            hp -= decrease_elements;
        }
        string enemy_side = side == "blue" ? "red" : "blue";

        cout << get_format_clock() << " " << side << " " << name << " " << id 
             << " reached " << enemy_side << " headquarter with " << hp 
             << " elements and force " << force << endl;
        cout << get_format_clock() << " " << enemy_side << " headquarter was taken\n";
        
        if (enemy_side == "blue")
            blue_conquered = true;
        else
            red_conquered = true;
    }

    void sort_weapons(bool used_first) {
        int sword_cnt = 0, bomb_cnt = 0, new_arrow_cnt = 0, used_arrow_cnt = 0;
        for (int i = 0; i < 10; i++) {
            weapon* p = weapons[i];
            if (p == nullptr) continue;
            if (p->name == "sword") {
                sword_cnt++;
            } else if (p->name == "bomb") {
                bomb* b = dynamic_cast<bomb*>(p);
                if (b->is_usable()) {
                    bomb_cnt++;
                }
                delete p;
            } else {
                arrow* a = dynamic_cast<arrow*>(p);
                if (a->use_count == 0) {
                    new_arrow_cnt++;
                } else if (a->use_count == 1) {
                    used_arrow_cnt++;
                }
                delete p;
            }
            weapons[i] = nullptr;
        }
        int ind = 0;
        while (sword_cnt--) weapons[ind++] = sw;
        while (bomb_cnt--) weapons[ind++] = new bomb("bomb");
        if (!used_first) {
            while (new_arrow_cnt--) weapons[ind++] = new arrow("arrow", 0);
            while (used_arrow_cnt--) weapons[ind++] = new arrow("arrow", 1);
        } else {
            while (used_arrow_cnt--) weapons[ind++] = new arrow("arrow", 1);
            while (new_arrow_cnt--) weapons[ind++] = new arrow("arrow", 0);
        }
        cur_weapon_ind = 0;
    }

    int count_weapons() {
        int cnt = 0;
        for (weapon* p : weapons) {
            if (p != nullptr) cnt++;
            else break;
        }
        return cnt;
    }

    void clear_weapons() {
        for (int i = 0; i < 10; i++) {
            if (weapons[i] != nullptr && !weapons[i]->is_usable()) {
                delete weapons[i];
                weapons[i] = nullptr;
            }
        }
    }

    void take_weapons_after_fight(warrior* enemy) {
        if (this == enemy) return;
        if (hp <= 0) return;
        enemy->clear_weapons();
        enemy->sort_weapons(false);
        clear_weapons();
        sort_weapons(true);
        int tot_weapon = count_weapons();
        int ind = 0;
        for (int i = tot_weapon; i < 10; i++) {
            weapons[i] = enemy->weapons[ind++];
        }
        sort_weapons(true);
    }

    bool is_alive() { return hp > 0; }

    weapon* find_proper_weapon() {
        weapon* res = weapons[cur_weapon_ind];
        if (res != nullptr && res->is_usable()) {
            cur_weapon_ind = (cur_weapon_ind + 1) % 10;
            return res;
        } else {
            int weapon_ind = (cur_weapon_ind + 1) % 10;
            while (weapon_ind != cur_weapon_ind) {
                res = weapons[weapon_ind];
                if (res != nullptr && res->is_usable()) {
                    cur_weapon_ind = (weapon_ind + 1) % 10;
                    return res;
                } else {
                    weapon_ind = (weapon_ind + 1) % 10;
                }
            }
        }
        return nullptr;
    }

    void attack(warrior* enemy) {
        weapon* w = find_proper_weapon();
        if (w == nullptr) return;
        w->attack(this->force, this->name, this->hp, enemy->hp);
    }
};

class dragon : public warrior {
public:
    dragon(warrior& w) : warrior(w) {}
    void yell() {
        cout << get_format_clock() << " " << side << " " << name << " " << id 
             << " yelled in city " << location << endl;
    }
};

class lion : public warrior {
public:
    int loyalty;
    lion(warrior& w, int loyalty_) : warrior(w), loyalty(loyalty_) {}
    
    void march() override {
        int direction = side == "red" ? 1 : -1;
        location += direction;
        loyalty -= loyaltyDecrease;
        cout << get_format_clock() << " " << side << " " << name << " " << id 
             << " marched to city " << location << " with " << hp 
             << " elements and force " << force << endl;
    }
    
    bool escape() {
        if (loyalty > 0) return false;
        else {
            cout << get_format_clock() << " " << side << " lion " << id << " ran away\n";
            return true;
        }
    }
};

class iceman : public warrior {
public:
    iceman(warrior& w) : warrior(w) {}
    void march() override {
        int direction = side == "red" ? 1 : -1;
        location += direction;
        int decrease_elements = hp / 10;
        hp -= decrease_elements;
        cout << get_format_clock() << " " << side << " " << name << " " << id 
             << " marched to city " << location << " with " << hp 
             << " elements and force " << force << endl;
    }
};

class wolf : public warrior {
public:
    wolf(warrior& w) : warrior(w) {}
    void rob_weapons(warrior* enemy) {
        if (enemy->name == "wolf") return;
        if (enemy == this) return;
        enemy->clear_weapons();
        enemy->sort_weapons(false);
        clear_weapons();
        sort_weapons(true);

        weapon* first_weapon = enemy->weapons[0];
        if (first_weapon == nullptr) return;

        string first_weapon_name = first_weapon->name;
        int self_ind = count_weapons(), enemy_ind = 0;
        for (weapon* p : enemy->weapons) {
            if (p == nullptr || p->name != first_weapon_name) break;
            weapons[self_ind++] = p;
            enemy->weapons[enemy_ind++] = nullptr;
        }
        enemy->sort_weapons(false);
        sort_weapons(true);
        cout << get_format_clock() << " " << side << " " << name << " " << id 
             << " took " << enemy_ind << " " << first_weapon_name 
             << " from " << enemy->side << " " << enemy->name << " " << enemy->id 
             << " in city " << location << endl;
    }
};

class ninja : public warrior {
public:
    ninja(warrior& w) : warrior(w) {}
};

// ---------------- 城市与基地 ----------------
class city {
public:
    warrior* first;
    warrior* second;
    city(warrior* red, warrior* blue) : first(red), second(blue) {}
};

vector<city> cities; // 城市全局向量

// 由于全局函数 warrior_born 需要在 headquarter 内部被调用，提前声明
void warrior_born(warrior& w);

class headquarter {
public:
    string side;
    int totalLife;
    int totalCount;
    int currentIdx;

    headquarter(string side_, int totalLife_)
        : side(side_), totalLife(totalLife_), totalCount(0), currentIdx(0) {}

    warrior* make_warrior() {
        warrior* ret = nullptr;
        string w_name = buildOrder[side][currentIdx];
        int cost = initialHPs[w_name];

        if (cost <= totalLife) {
            totalCount++;
            int warrior_id = totalCount;
            int location = (side == "red") ? 0 : cityCount + 1;
            totalLife -= cost;
            currentIdx = (currentIdx + 1) % 5;
            
            ret = new warrior(warrior_id, cost, side, w_name, location, initialForces[w_name]);
        }
        return ret;
    }

    weapon* make_weapon(string name) {
        if (name == "sword") return sw;
        if (name == "bomb") return new bomb(name);
        if (name == "arrow") return new arrow(name, 0);
        return nullptr;
    }

    warrior* further_process(warrior* p) {
        if (p == nullptr) return nullptr;
        
        string name = p->name;
        warrior* ret = nullptr;
        
        if (name == "dragon") {
            string weapon_name = weapon_kinds[p->id % 3];
            p->weapons[0] = make_weapon(weapon_name);
            ret = new dragon(*p);
        } else if (name == "ninja") {
            string weapon_name1 = weapon_kinds[p->id % 3];
            string weapon_name2 = weapon_kinds[(p->id + 1) % 3];
            p->weapons[0] = make_weapon(weapon_name1);
            p->weapons[1] = make_weapon(weapon_name2);
            ret = new ninja(*p);
        } else if (name == "iceman") {
            string weapon_name = weapon_kinds[p->id % 3];
            p->weapons[0] = make_weapon(weapon_name);
            ret = new iceman(*p);
        } else if (name == "lion") {
            string weapon_name = weapon_kinds[p->id % 3];
            p->weapons[0] = make_weapon(weapon_name);
            ret = new lion(*p, totalLife);
        } else if (name == "wolf") {
            ret = new wolf(*p);
        }
        return ret;
    }

    void make_warriors() {
        warrior* p = further_process(make_warrior());
        if (p != nullptr) {
            warrior_born(*p);
            if (p->side == "red") {
                cities[0].first = p;
            } else {
                cities[cities.size() - 1].second = p;
            }
        }
    }
};


// ---------------- 全局统筹事件函数组 ----------------
void warrior_born(warrior& w) {
    cout << get_format_clock() << " " << w.side << " " << w.name << " " << w.id << " born\n";
    if (w.name == "lion") {
        lion* l = dynamic_cast<lion*>(&w);
        cout << "Its loyalty is " << l->loyalty << endl;
    }
}

void happen00_warrior_born(headquarter& red, headquarter& blue) {
    red.make_warriors();
    blue.make_warriors();
}

void happen05_lion_escape() {
    for (int i = 0; i < cities.size(); i++) {
        warrior *first = cities[i].first, *second = cities[i].second;
        if (first != nullptr && first->name == "lion") {
            lion* l = dynamic_cast<lion*>(first);
            if (l->escape()) {
                delete cities[i].first;
                cities[i].first = nullptr;
            }
        }
        if (second != nullptr && second->name == "lion") {
            lion* l = dynamic_cast<lion*>(second);
            if (l->escape()) {
                delete cities[i].second;
                cities[i].second = nullptr;
            }
        }
    }
}

void happen10_warrior_march() {
    warrior* b = cities[1].second;
    if (b != nullptr) {
        b->reach_headquarter();
        delete cities[1].second;
        cities[1].second = nullptr;
    }

    warrior* next_move_red = cities[0].first;
    cities[0].first = nullptr;

    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = next_move_red, *b = cities[i + 1].second;
        next_move_red = cities[i].first;
        if (r != nullptr) r->march();
        cities[i].first = r;

        if (b != nullptr) b->march();
        cities[i].second = b;
    }
    cities[cities.size() - 1].second = nullptr;
    if (next_move_red != nullptr) {
        next_move_red->reach_headquarter();
        delete next_move_red;
    }
}

void happen35_wolf_rob() {
    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = cities[i].first, *b = cities[i].second;
        if (r == nullptr || b == nullptr) continue;
        
        if (r->name == "wolf") {
            wolf* w = dynamic_cast<wolf*>(r);
            w->rob_weapons(b);
        }
        if (b->name == "wolf") {
            wolf* w = dynamic_cast<wolf*>(b);
            w->rob_weapons(r);
        }
    }
}

fight_result trigger_fight(warrior* first, warrior* second) {
    int still_epoch = 0;
    while (first->is_alive() && second->is_alive()) {
        int first_elements = first->hp, second_elements = second->hp;
        if (first->is_alive()) first->attack(second);
        if (second->is_alive()) second->attack(first);
        
        if (first_elements == first->hp && second_elements == second->hp) {
            still_epoch++;
        }
        if (still_epoch >= 12) return BOTH_ALIVE;
    }
    warrior* alived_warrior = first->is_alive() ? first : second;
    if (!alived_warrior->is_alive()) return BOTH_DEAD;
    else {
        if (alived_warrior->side == "red") return RED_WIN;
        else return BLUE_WIN;
    }
}

void happen40_war() {
    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = cities[i].first, *b = cities[i].second;
        if (r == nullptr || b == nullptr) continue;
        
        r->clear_weapons();
        r->sort_weapons(true);
        b->clear_weapons();
        b->sort_weapons(true);
        
        fight_result res;
        if (i % 2) res = trigger_fight(r, b);
        else res = trigger_fight(b, r);
        
        dragon* rd = dynamic_cast<dragon*>(r);
        dragon* bd = dynamic_cast<dragon*>(b);
        
        if (res == BOTH_ALIVE) {
            cout << get_format_clock() << " both red " << r->name << " " << r->id 
                 << " and blue " << b->name << " " << b->id << " were alive in city " << r->location << endl;
            if (rd != nullptr) rd->yell();
            if (bd != nullptr) bd->yell();
        } else if (res == BOTH_DEAD) {
            cout << get_format_clock() << " both red " << r->name << " " << r->id 
                 << " and blue " << b->name << " " << b->id << " died in city " << r->location << endl;
        } else if (res == RED_WIN) {
            r->take_weapons_after_fight(b);
            cout << get_format_clock() << " red " << r->name << " " << r->id 
                 << " killed blue " << b->name << " " << b->id << " in city " << r->location 
                 << " remaining " << r->hp << " elements\n";
            if (rd != nullptr) rd->yell();
        } else {
            b->take_weapons_after_fight(r);
            cout << get_format_clock() << " blue " << b->name << " " << b->id 
                 << " killed red " << r->name << " " << r->id << " in city " << b->location 
                 << " remaining " << b->hp << " elements\n";
            if (bd != nullptr) bd->yell();
        }
        
        r->clear_weapons();
        r->sort_weapons(true);
        b->clear_weapons();
        b->sort_weapons(true);
        
        if (!r->is_alive()) {
            delete cities[i].first;
            cities[i].first = nullptr;
        }
        if (!b->is_alive()) {
            delete cities[i].second;
            cities[i].second = nullptr;
        }
    }
}

void report_base(headquarter& base) {
    cout << get_format_clock() << " " << base.totalLife << " elements in " << base.side << " headquarter\n";
}

void happen50_report_base(headquarter& red, headquarter& blue) {
    report_base(red);
    report_base(blue);
}

void report_warrior(warrior* w) {
    if (w == nullptr) return;
    int sword_cnt = 0, bomb_cnt = 0, arrow_cnt = 0;
    w->clear_weapons();
    w->sort_weapons(true);
    for (weapon* p : w->weapons) {
        if (p == nullptr) continue;
        if (p->name == "sword") sword_cnt++;
        if (p->name == "bomb") bomb_cnt++;
        if (p->name == "arrow") arrow_cnt++;
    }
    cout << get_format_clock() << " " << w->side << " " << w->name << " " << w->id 
         << " has " << sword_cnt << " sword " << bomb_cnt << " bomb " << arrow_cnt 
         << " arrow and " << w->hp << " elements\n";
}

void happen55_report_warriors() {
    for (int i = 1; i < cities.size() - 1; i++) {
        warrior *r = cities[i].first, *b = cities[i].second;
        report_warrior(r);
        report_warrior(b);
    }
}

// ---------------- 游戏初始化逻辑 ----------------
void init_game() {
    current_time = 0;
    blue_conquered = false;
    red_conquered = false;
    
    buildOrder["blue"] = {"lion", "dragon", "ninja", "iceman", "wolf"};
    buildOrder["red"] = {"iceman", "lion", "wolf", "ninja", "dragon"};
    
    if (sw == nullptr) sw = new sword("sword");
    
    for (city& c : cities) {
        if (c.first != nullptr) delete c.first;
        if (c.second != nullptr) delete c.second;
    }
    cities.clear();
    for (int i = 0; i < cityCount + 2; i++) {
        cities.push_back(city(nullptr, nullptr));
    }
}

int main() {
    int tot;
    if (!(cin >> tot)) return 0;
    
    for (int c = 1; c <= tot; c++) {
        printf("Case %d:\n", c);
        int tot_elements;
        cin >> tot_elements >> cityCount >> loyaltyDecrease >> timeLimit;
        
        cin >> initialHPs["dragon"] >> initialHPs["ninja"] 
            >> initialHPs["iceman"] >> initialHPs["lion"] 
            >> initialHPs["wolf"];
            
        cin >> initialForces["dragon"] >> initialForces["ninja"] 
            >> initialForces["iceman"] >> initialForces["lion"] 
            >> initialForces["wolf"];
            
        init_game();
        
        headquarter red("red", tot_elements);
        headquarter blue("blue", tot_elements);
        
        while (current_time <= timeLimit && !blue_conquered && !red_conquered) {
            if (current_time % 60 == 0) happen00_warrior_born(red, blue);
            else if (current_time % 60 == 5) happen05_lion_escape();
            else if (current_time % 60 == 10) happen10_warrior_march();
            else if (current_time % 60 == 35) happen35_wolf_rob();
            else if (current_time % 60 == 40) happen40_war();
            else if (current_time % 60 == 50) happen50_report_base(red, blue);
            else if (current_time % 60 == 55) happen55_report_warriors();
            
            current_time += 5;
        }
    }
    return 0;
}