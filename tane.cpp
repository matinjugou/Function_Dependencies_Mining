//
// Created by 黄超 on 2018/5/22.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <set>

using namespace std;
unsigned long N_ATTR;
unsigned long N_DATA;

vector<vector<string>> data;
vector<vector<int>> L(2);
int T[110000];
vector<vector<int>> S(110000);
int RHS[35000];
vector<vector<int>> partitions[35000];
bool computed[35000];

int reverseInt(int a) {
    int ans = 0;
    for (int i = 0; i < N_ATTR; i++) {
        ans = (ans << 1) + (1 & a);
        a = a >> 1;
    }
    return ans;
}

struct ansEle
{
    pair<int, int> ele;
    ansEle(int a, int b) {
        ele = pair<int, int>(a, b);
    }
    bool operator <(const ansEle &a)const //排序并且去重复
    {
        if (reverseInt(ele.first) > reverseInt(a.ele.first)) return true;
        else if (reverseInt(ele.first) == reverseInt(a.ele.first)) {
            if (reverseInt(ele.second) > reverseInt(a.ele.second)) return true;
        }
        return false;
    }

};

set<ansEle> ans;

vector<string> splitString(string str, char split) {
    vector<string> ans;
    unsigned long len, p;
    while (true) {
        len = str.length();
        p = 0;
        while (true)
        {
            while (str[p] != split && p < len) p++;
            if (p == len) {
                ans.push_back(str);
                return ans;
            }
            if (str[p] == split && str[p + 1] == ' ') {
                p += 1;
                continue;
            }
            break;
        }
        ans.push_back(str.substr(0, p));
        str = str.substr(p + 1, len - (p + 1));
    }
}

void scanData(char* filename) {
    ifstream infile;
    infile.open(filename, ios::in);
    if(!infile) {
        cout<<"Error when open file!"<<endl;
        return;
    }
    string tmpStr;
    while (!infile.eof()) {
        getline(infile, tmpStr);
        data.push_back(splitString(tmpStr, ','));
    }
    infile.close();
    N_ATTR = data[0].size();
    N_DATA = data.size();
}

void printDependency() {
    for (auto &pair : ans) {
        int left = pair.ele.first;
        int right = pair.ele.second;
        for (int A = 0; A < N_ATTR; A++) {
            if ((1 << A) & left) {
                cout << A + 1 << " ";
            }
        }
        for (int A = 0; A < N_ATTR; A++) if ((1 << A) & right) {
                cout << "-> " << A + 1 << endl;
            }
    }
}

bool judgePrefix(int A, int B) {
    bool flag1 = false, flag2 = false;
    int bit = 0;
    int XOR = A ^ B;
    for (int i = N_ATTR - 1; i >= 0; i--) {
        bit = 1 << i;
        if (XOR & bit) {
            if (!flag1 && !flag2) {
                flag1 = true;
            } else if (flag1 && !flag2){
                flag2 = true;
            } else {
                return false;
            }
        } else {
            if (A & bit) {
                if (!flag1 && !flag2) {
                    continue;
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

void strippedProduct(int A, int B) {
    vector<vector<int>> tmpV;
    partitions[A | B].clear();
    int i = 0;
    for (auto &ele: partitions[A]) {
        for (auto &t: ele) T[t] = i;
        S[i].clear();
        i += 1;
    }
    i = 0;
    for (auto &ele: partitions[B]) {
        for (auto &t: ele) {
            if (T[t] != -1) S[T[t]].push_back(t);
        }
        for (auto &t: ele) {
            if (S[T[t]].size() >= 2) {
                tmpV.push_back(S[T[t]]);
            }
            S[T[t]].clear();
        }
        i += 1;
    }
    for (auto &ele: partitions[A]) {
        for (auto &t: ele) T[t] = -1;
    }
    partitions[A | B] = tmpV;
}

bool judgeRelation(int A, int B) {
    if (A == 0) return false;
    if (!computed[A | B]) {
        strippedProduct(A, B);
        computed[A | B] = true;
    }
    return partitions[A].size() == partitions[A | B].size();
}

void computeDependencies(int l) {
    for (auto &X: L[l]) {
        bool flag = false;
        for (int i = 0; i < N_ATTR; i++) {
            if (((1 << i) & X) > 0) {
                if (!flag) {
                    flag = true;
                    RHS[X] = RHS[X - (1 << i)];
                } else {
                    RHS[X] = RHS[X] & RHS[X - (1 << i)];
                }
            }
        }
    }
    for (auto &X: L[l]) {
        int Merge_X = X & RHS[X];
        int attr = 0;
        for (int A = N_ATTR - 1; A >= 0; A--) {
            attr = 1 << A;
            if (attr & Merge_X) {
                if (judgeRelation(X - attr, attr)) {
                    // printDependency(X - attr, attr);
                    ans.insert(ansEle(X - attr, attr));
                    RHS[X] -= attr;
                    for (int B = 0; B < N_ATTR; B++) {
                        if (!((1 << B) & X) && ((1 << B) & RHS[X])) RHS[X] -= (1 << B);
                    }
                }
            }
        }
    }
}

void generateNextLevel(int l) {
    vector<int> tmpL;
    bool flag1, flag2;
    int length = L[l].size();
    int attr = 0;
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            if (judgePrefix(L[l][i], L[l][j])) {
                int X = L[l][i] | L[l][j];
                flag1 = true;
                for (int A = 0; A < N_ATTR; A++) {
                    flag2 = false;
                    attr = 1 << A;
                    if (X & attr) {
                        for (auto &x: L[l]) {
                            if (x == (X - attr)) {
                                flag2 = true;
                                break;
                            }
                        }
                        if (!flag2) {
                            flag1 = false;
                            break;
                        }
                    }
                }
                if (flag1) {
                    tmpL.emplace_back(X);
                }
            }
        }
    }
    L.emplace_back(tmpL);
}

void prune(int l) {
    vector<int> tmpL;
    for (auto &X: L[l]) {
        if (RHS[X] != 0) {
            tmpL.emplace_back(X);
        }
    }
    L[l] = tmpL;
}

void init() {
    for (int i = 0; i < 35000; i++) {
        RHS[i] = 0;
        computed[i] = false;
    }
    partitions[0].clear();
    L[0].clear();
    RHS[0] = (1 << N_ATTR) - 1;
    for (int A = 0; A < N_ATTR; A++) {
        L[1].push_back(1 << A);
    }
    for (int i = 0; i < 110000; i++) {
        T[i] = -1;
    }
    unordered_map<string, vector<int>> rowSet;
    for (int A = 0; A < N_ATTR; A++) {
        rowSet.clear();
        for (int i = 0; i < N_DATA; i++) {
            rowSet[data[i][A]].push_back(i);
        }
        for (auto &pair: rowSet) {
            if (pair.second.size() >= 2) partitions[1 << A].push_back(pair.second);
        }
        computed[1 << A] = true;
    }
}

int main(int argc, char** argv) {
    scanData(argv[1]);
    init();
    int l = 1;
    while (!L[l].empty()) {
        computeDependencies(l);
        prune(l);
        generateNextLevel(l);
        l += 1;
    }
    printDependency();
    return 0;
}