/*
描述
为了迎接08年的奥运会，让大家更加了解各种格斗运动，facer新开了一家热血格斗场。格斗场实行会员制，
但是新来的会员不需要交入会费，而只要同一名老会员打一场表演赛，证明自己的实力。

我们假设格斗的实力可以用一个正整数表示，成为实力值。另外，每个人都有一个唯一的id，也是一个正整数。
为了使得比赛更好看，每一个新队员都会选择与他实力最为接近的人比赛，即比赛双方的实力值之差的绝对值越小越好，
如果有两个人的实力值与他差别相同，则他会选择比他弱的那个（显然，虐人必被虐好）。

不幸的是，Facer一不小心把比赛记录弄丢了，但是他还保留着会员的注册记录。
现在请你帮facer恢复比赛纪录，按照时间顺序依次输出每场比赛双方的id。

输入
第一行一个数n(0 < n <=100000)，表示格斗场新来的会员数（不包括facer）。
以后n行每一行两个数，按照入会的时间给出会员的id和实力值。
一开始，facer就算是会员，id为1，实力值1000000000。输入保证两人的实力值不同。

输出
N行，每行两个数，为每场比赛双方的id，新手的id写在前面。

样例输入
3
2 1
3 3
4 2
样例输出
2 1
3 2
4 2
*/
#include <iostream>
#include <map>
#include <cmath>

using namespace std;

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    // key: 实力值, value: id
    map<int, int> record;
    record[1000000000] = 1;

    for (int i = 0; i < n; ++i) {
        int id, strength;
        cin >> id >> strength;

        // 查找第一个大于等于当前实力值的人
        auto it = record.lower_bound(strength);

        int opponent_id;
        if (it == record.begin()) {
            // 没有更弱的人，只能选当前这个最接近的（更强）
            opponent_id = it->second;
        } else if (it == record.end()) {
            // 没有更强的人，只能选前一个（更弱）
            opponent_id = prev(it)->second;
        } else {
            // 有更强也有更弱，比较差值
            auto prev_it = prev(it);
            if (abs(it->first - strength) < abs(prev_it->first - strength)) {
                opponent_id = it->second;
            } else {
                // 差值相同选弱的，或者确实是弱的更近
                opponent_id = prev_it->second;
            }
        }

        cout << id << " " << opponent_id << "\n";
        record[strength] = id;
    }

    return 0;
}
