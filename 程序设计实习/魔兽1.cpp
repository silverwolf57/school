#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

using namespace std;

const string WARRIOR_NAMES[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};

class Headquarter {
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
    Headquarter(string s, int m, vector<int> order, const vector<int>& hps) 
        : side(s), totalLife(m), buildOrder(order), initialHPs(hps) {
        totalCount = 0;
        currentIdx = 0;
        stopped = false;
        for(int i=0; i<5; ++i) countMap[WARRIOR_NAMES[i]] = 0;
    }

    bool isStopped() const { return stopped; }

    void tryBuild(int hour) {
        if (stopped) return;

        for (int i = 0; i < 5; ++i) {
            int warriorIdx = buildOrder[currentIdx];
            int cost = initialHPs[warriorIdx];
            string name = WARRIOR_NAMES[warriorIdx];

            if (totalLife >= cost) {
                totalLife -= cost;
                totalCount++;
                countMap[name]++;
                
                cout << setfill('0') << setw(3) << hour << " " << side << " " << name << " " 
                     << totalCount << " born with strength " << cost << "," << countMap[name] 
                     << " " << name << " in " << side << " headquarter" << endl;

                currentIdx = (currentIdx + 1) % 5;
                return;
            }
            currentIdx = (currentIdx + 1) % 5;
        }
        stopped = true;
        cout << setfill('0') << setw(3) << hour << " " << side << " headquarter stops making warriors" << endl;
    }
};

int main() {
    int testCases;
    if (!(cin >> testCases)) return 0;
    for (int t = 1; t <= testCases; ++t) {
        int M;
        cin >> M;
        vector<int> hps(5);
        for (int i = 0; i < 5; ++i) cin >> hps[i];
        vector<int> redOrder = {2, 3, 4, 1, 0};
        vector<int> blueOrder = {3, 0, 1, 2, 4};

        Headquarter red("red", M, redOrder, hps);
        Headquarter blue("blue", M, blueOrder, hps);

        cout << "Case:" << t << endl;

        int hour = 0;
        while (!red.isStopped() || !blue.isStopped()) {
            red.tryBuild(hour);
            blue.tryBuild(hour);
            hour++;
        }
    }
    return 0;
}