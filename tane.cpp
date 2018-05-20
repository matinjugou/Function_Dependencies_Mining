#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<string>> data;
unordered_map<set<int>, set<set<int>>> piMap;
unsigned long totalAttr;

struct dependency {
    set<int> left;
    int right;
};

vector<vector<dependency>> level;

int currentLevel;

vector<string> splitString(string str, char split) {
    vector<string> ans;
    unsigned long len, p;
    while (true) {
        len = str.length();
        p = 0;
        while (str[p] != split && p < len) p++;
        if (p == len) {
            ans.push_back(str);
            return ans;
        }
        ans.push_back(str.substr(0, p));
        str = str.substr(p + 1, len - (p + 1));
    }
}

void scanData(char *filename) {
    ifstream infile;
    infile.open(filename, ios::in);
    if (!infile) {
        cout << "Error when open file!" << endl;
        return;
    }
    string tmpstr;
    while (!infile.eof()) {
        getline(infile, tmpstr);
        data.push_back(splitString(tmpstr, ','));
    }
    infile.close();
    totalAttr = data[0].size();
}

set<set<int>> getPI1(int attrNum) {
    unordered_map<string, set<int>> uMap;
    unordered_map<string, set<int>>::iterator it;
    set<set<int>> PI1;
    int rowNum = 0;
    for (auto &row :data) {
        if (uMap[attrNum].count(row[attrNum]) == 0) {
            set<int> arr;
            uMap[attrNum].insert(make_pair(row[attrNum], arr));
        }
        uMap[attrNum][row[attrNum]].insert(rowNum);
        rowNum++;
    }
    for(auto& UM: uMap) {
        PI1.insert(UM.second);
        UM.second.clear();
    }
    uMap.clear();
    return PI1;
}
/*
 * How to use getPI1?
 *
 * for ( int i = 0; i<attrNum; i++) {
 *     set<int> newSet;
 *     newSet.insert(i);
 *     piMap.insert(make_pair(newSet, getPI1(i)));
 * }
 *
 * */

vector<vector<dependency>> prefixBlocks() {
    vector<vector<dependency>> blocks;
    vector<dependency> tmpLevel = level[currentLevel % 2];
    for (auto &Y: tmpLevel) {
        for (auto &Z: tmpLevel) {
            if (Y.left == Z.left && Y.right != Z.right) {
                vector<dependency> tmp;
                tmp.push_back(Y);
                tmp.push_back(Z);
                blocks.push_back(tmp);
            }
        }
        tmpLevel.erase(tmpLevel.begin());
    }
    return blocks;
}

void strippedProduct(dependency X, dependency Y) {
    for(auto& n: piMap[X.left]) {
        for(auto& t: n){

        }
    }
    for(auto& m: piMap[Y.left]) {

    }
}

void generateNextLevel() {
    vector<dependency>::iterator it;
    vector<dependency> curLevel = level[currentLevel % 2];
    vector<dependency> newLevel;
    for (auto &K: prefixBlocks()) {
        set<int> nextAll = K[0].left;
        nextAll.insert(K[0].right);
        nextAll.insert(K[1].right);

        set<int> tempAll = nextAll;
        for (auto &tmp :nextAll) {
            tempAll = nextAll;

            tempAll.erase(tmp);
            dependency tmpDep;
            tmpDep.left = tempAll;
            tmpDep.right = tmp;

            it = find(curLevel.begin(), curLevel.end(), tmpDep);
            if (it == curLevel.end())
                break;

            newLevel.push_back(tmpDep);
        }
        if (it != curLevel.end()) {
            strippedProduct(K[0],K[1]);
            for (auto &dep :newLevel) {
                level[(currentLevel + 1) % 2].push_back(dep);
            }
        }
    }
}


set<int> findRHSCandidate(set<int> attrW) {
    set<int> RHSCandidate;
    set<int>::iterator it;
    for (int i = 0; i < totalAttr; i++) {
        if (attrW.find(i) != attrW.end()) {
            attrW.erase(i);
            if (judgeRelation(attrW, i)) {
                RHSCandidate.insert(i);
            }
            attrW.insert(i);
        } else {
            RHSCandidate.insert(i);
        }
    }
}