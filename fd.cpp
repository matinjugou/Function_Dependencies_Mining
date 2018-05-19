#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>

using namespace std;

vector<vector<string>> data;
unsigned long totalAttr;

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

void scanData(char* filename) {
    ifstream infile;
    infile.open(filename, ios::in);
    if(!infile) {
        cout<<"Error when open file!"<<endl;
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

bool judgeRelation(set<int> leftList, int right) {
    set<string> attrSet;
    set<string> attrSet2;
    string tmpLine;
    for (auto &line: data) {
        tmpLine = "";
        for (auto &attr: leftList) {
            tmpLine += line[attr];
        }
        attrSet.insert(tmpLine);
        tmpLine += line[right];
        attrSet2.insert(tmpLine);
    }
    return attrSet.size() == attrSet2.size();
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

int main(int argc, char** argv) {
    cout<<"Hello World "<<argv[1]<<endl;
    scanData(argv[1]);
    return 0;
}