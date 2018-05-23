//
// Created by 黄超 on 2018/5/21.
//

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

struct level_element {
    set<int> element;
    set<int> RHS;
    level_element(set<int> &ele) {
        element = ele;
    }
};

vector<set<level_element>> L;
// RHS_level[current_level % 2] refers to current level

set<int> R;

set<pair<set<int>, int>> answers;

vector<set<set<int>>> partitions;
vector<int> T;
vector<set<int>> S;

int current_level;

void computeDependecies(int l) {
    for (auto &ele: L[l % 2]) {
        set<int> tmpC;
        vector<int> compare_res;
        tmpC.clear();
        for (auto &old_ele: L[(l + 1) % 2]) {
            set_difference(ele.element.begin(),
                           ele.element.end(),
                           old_ele.element.begin(),
                           old_ele.element.end(),
                           insert_iterator<vector<int>>(compare_res, compare_res.begin()));
            if (compare_res.size() == 1) {
                set_intersection(tmpC.begin(),
                                 tmpC.end(),
                                 old_ele.RHS.begin(),
                                 old_ele.RHS.end(),
                                 insert_iterator<set<int>>(ele.RHS, ele.RHS.begin()));
                tmpC = ele.RHS;
            }
            compare_res.clear();
        }
    }
    for (auto &ele: L[l % 2]) {
        vector<int> tmpV;
        set_intersection(ele.element.begin(),
                         ele.element.end(),
                         ele.RHS.begin(),
                         ele.RHS.end(),
                         insert_iterator<vector<int>>(tmpV, tmpV.begin()));
        set<int> tmpS = ele.element;
        for (auto &i: tmpV) {
            tmpS.erase(i);
            if (judgeRelation(tmpS, i)) {
                answers.insert(pair(tmpS, i));
                ele.RHS.erase(i);
                for (auto &x: R) {
                    if (ele.element.find(x) == ele.element.end()) {
                        ele.RHS.erase(x);
                    }
                }
            }
            tmpS.insert(i);
        }
    }
}

int judgePrefix(set<int> &set1, set<int> &set2) {
    set<int>::iterator it1, it2;
    it1 = set1.begin();
    it2 = set2.begin();
    int count = 0;
    while (it1 != set1.end() && it2 != set2.end() && *it1 == *it2) {
        count++;
        it1++;
        it2++;
    }
    return count;
}

struct candidateLevelElement {
    set<int> element;
    int count;
    candidateLevelElement(set<int> &ele, int cou) {
        element = ele;
        count = cou;
    }
};

void generate_next_level(int l) {
    L[(l + 1) % 2].clear();
    set<set<int>> tmpOldLevel;
    set<set<int>> tmpLevel;
    set<int>::iterator itS;
    set<int> tmpS;
    for (auto &i: L[l % 2]) {
        for (auto &j: L[l % 2]) {
            if (i.element != j.element) {
                if (judgePrefix(i.element, j.element) == (i.element.size() - 1)) {
                    tmpS.clear();
                    set_union(i.element.begin(),
                              i.element.end(),
                              j.element.begin(),
                              j.element.end(),
                              insert_iterator<set<int>>(tmpS, tmpS.begin()));
                    tmpLevel.insert(tmpS);
                }
            }
        }
    }
    for (auto &ele: L[l % 2]) {
        tmpOldLevel.insert(ele.element);
    }
    bool flag;
    for (auto &ele: tmpLevel) {
        flag = true;
        tmpS.clear();
        tmpS = ele;
        for (auto i: ele) {
            tmpS.erase(i);
            if (tmpOldLevel.find(tmpS) == tmpOldLevel.end()) {
                flag = false;
                break;
            }
            tmpS.insert(i);
        }
        if (flag) {
            L[(l + 1) % 2].insert(level_element(ele));
        }
    }
}

void prune(int l) {
    set<level_element> tmpLevel;
    for (auto &ele: L[l % 2]) {
        if (!ele.RHS.empty()) {
            tmpLevel.insert(ele);
        }
    }
    L[l % 2] = tmpLevel;
}

void init () {
    // TODO: init all variables
}

int main() {
    set<int> a = {1, 3, 4};
    set<int> b = {1, 3, 4};
    cout << (a == b);
    return 0;
}