#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;


int main() {
    ifstream puzzleFile("puzzle.txt");
    string str;
    vector<string> puzzle;
    std::unordered_set<size_t> indices;
    long long splitCounter = 0;
    std::getline(puzzleFile, str);
    size_t startIndex = str.find_first_of("S");
    indices.insert(startIndex);
    puzzle.push_back(str);
    while (std::getline(puzzleFile, str)) {
        if (str.find("^") != std::string::npos) {
            puzzle.push_back(str);
        }
        std::unordered_set<size_t> newIndices;
        std::unordered_set<size_t> eraseIndices;
        if (str.empty()) {
            continue;
        }
        for (auto i : indices) {
            if (str[i] == '^') {
                newIndices.insert(i - 1);
                newIndices.insert(i + 1);
                eraseIndices.insert(i);
                splitCounter++;
            }
        }
        for (auto r : eraseIndices) {
            indices.erase(r);
        }
        indices.insert(newIndices.begin(), newIndices.end());
    }
    cout << splitCounter << endl;
    
    vector<unsigned long long> accumulator;
    string line = puzzle[puzzle.size() - 1];
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '^') {
            accumulator.push_back(2);
        } else {
            accumulator.push_back(1);
        }
    }


    for (int i = puzzle.size() - 2; 0 < i; i--) {
        line = puzzle[i];
        for (int j = 0; j < puzzle[i].size(); j++) {
            if (line[j] == '^') {
                accumulator[j] = accumulator[j - 1] + accumulator[j + 1];
            }
        }
    }
    cout << accumulator[startIndex] << endl;
}




