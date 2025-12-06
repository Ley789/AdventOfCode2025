#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
using namespace std;


inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

struct math_problem {
    math_problem(vector<string> input, bool add) : group(input), add(add) { }


    long long interpretRow() const {
        int nullElement = add ? 0 : 1;
        long long result = nullElement;
        for (int i = 0; i < group.size(); i++) {
            string line = group[i];
            long long val = interpreteString(line);
            if (add) {
                result += val;
            }
            else {
                result *= val;
            }
        }
        return result;
    }

    long long interpretColumn() const {
        int nullElement = add ? 0 : 1;
        long long result = nullElement;
        for (int i = 0; i < group[0].size(); i++) {
            string line;
            for (int j = 0; j < group.size(); j++) {
                line.push_back(group[j][i]);
            }
            long long val = interpreteString(line);
            if (add) {
                result += val;
            } else {
                result *= val;
            }
        }
        return result;
    }
private:
    const vector<string> group;
    const bool add;

    long long interpreteString(string& l) const {
        long long nullElement = add ? 0 : 1;
        ltrim(l);
        rtrim(l);
        if (l.empty()) {
            return nullElement;
        }
        return stoll(l);
    }
};



int main() {
    ifstream puzzleFile("puzzle.txt");
   
    
    vector<string> input;

    //parse input
    for (int i = 0; i < 4; i++) {
        string str;
        std::getline(puzzleFile, str);
        input.push_back(str);
    }

    //parse operators and build problem group
    string oprators;
    std::getline(puzzleFile, oprators);
    vector<math_problem> puzzle;
    while (!oprators.empty()) {
        char o = oprators[0];
        bool add;
        if (o == '+') {
            add = true;
        } else {
           add = false;
        }
        vector<string> group;
        size_t nextOpIndex = oprators.find_first_of("*+", 1);
        for (int i = 0; i < input.size(); i++) {
            string mathProblemNum = input[i].substr(0, nextOpIndex);
            group.push_back(mathProblemNum);
            input[i].erase(0, nextOpIndex);
        }
        oprators.erase(0, nextOpIndex);
        puzzle.push_back(math_problem(group, add));
    }

    long long result = 0;
    for (auto p : puzzle) {
        result += p.interpretRow();
    }
    cout << result << endl;;

    result = 0;
    for (auto p : puzzle) {
        result += p.interpretColumn();
    }
    cout << result;

}


