#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include <cmath>
using namespace std;


size_t findMaxForPosition(const vector<char>& puzzleRow, size_t start_index, unsigned int decimalPosition) {
    auto max = max_element(puzzleRow.begin() + start_index, puzzleRow.end() - decimalPosition);
    return std::distance(std::begin(puzzleRow), max);
}

int main() {
    ifstream puzzleFile("puzzle.txt");
    string str;
    int digitsPerRow = 12;

    vector<vector<char>> puzzle;
    while (std::getline(puzzleFile, str)) {
        vector<char> row;

        for (int i = 0; i < str.size(); i++) {
            row.push_back(str.at(i));
        }
        puzzle.push_back(row);
    }

    long long counter = 0;
    for (size_t i = 0; i < puzzle.size(); i++) {
        size_t currentMax = 0; 
        for (int d = digitsPerRow; d > 0; d--) {
            currentMax = findMaxForPosition(puzzle[i], currentMax, d - 1);
            counter += (puzzle[i][currentMax] - '0') * pow(10, d-1);
            currentMax++;
        }
    }
    cout << counter << std::endl;
}

