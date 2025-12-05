#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
using namespace std;

struct entry {
    unsigned long long lower;
    unsigned long long upper;
    entry(unsigned long long a, unsigned long long b) {
        if (a <= b) {
            lower = a;
            upper = b;
        }
        else {
            lower = b;
            upper = a;
        }
    }
};

bool greaterThen(entry e, unsigned long long value) {
    return e.upper < value;
}

unsigned long long min(unsigned long long a, unsigned long long b) {
    if (a <= b) {
        return a;
    }
    return b;
}

unsigned long long max(unsigned long long a, unsigned long long b) {
    if (a >= b) {
        return a;
    }
    return b;
}

int main() {
    ifstream inventoryFile("inventory.txt");
    ifstream ids("ids.txt");
    string str;
    string del = "-";
    vector<entry> inventoryV;
    while (std::getline(inventoryFile, str)) {
        auto pos = str.find(del);
        unsigned long long a = std::stoll(str.substr(0, pos));
        unsigned long long b = std::stoll(str.substr(pos+1));
        entry e = entry(a, b);
        size_t index = distance(begin(inventoryV), std::lower_bound(inventoryV.begin(), inventoryV.end(), e.lower, greaterThen));

        if (index < inventoryV.size()) {
            entry m = inventoryV[index];
            if (e.upper + 1 < m.lower) {
                inventoryV.insert(inventoryV.begin() + index, e);
            }
            else {
                e.lower = min(e.lower, m.lower);
                e.upper = max(e.upper, m.upper);
                inventoryV.at(index) = e;
            }
        }
        else {
            inventoryV.push_back(e);
        }
    }
    //merge list
    for (size_t i = 1; i < inventoryV.size(); i++) {
        auto p = inventoryV[i - 1];
        auto c = inventoryV[i];
        if (c.lower <= p.upper) {
            entry e = entry(p.lower, c.upper);
            inventoryV.at(i - 1) = e;
            inventoryV.erase(inventoryV.begin() + i );
            i--;
        }
    }

    long counter = 0;
    while (std::getline(ids, str)) {
        unsigned long long a = std::stoll(str);
        size_t index = distance(begin(inventoryV), std::lower_bound(inventoryV.begin(), inventoryV.end(), a, greaterThen));
        if (index < inventoryV.size()) {
            entry m = inventoryV[index];
            if (m.lower <= a) {
                counter++;
            }
        }
    }
    //part 1
    cout << counter << std::endl;

    unsigned long long freshCounter = 0;
    for (auto e : inventoryV) {
        freshCounter += e.upper - e.lower + 1;
    }
    cout << freshCounter << std::endl;
}