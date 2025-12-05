#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
using namespace std;

long removePaper(vector<vector<int>>& puzzle) {
    long counter = 0;
    for (size_t i = 0; i < puzzle.size(); i++) {
        for (size_t j = 0; j < puzzle[i].size(); j++) {

            int innerCounter = 0;
            if (puzzle[i][j] == 0) {
                continue;
            }
            vector<int> rowIndices;
            vector<int> columIndices;
            rowIndices.push_back(i);
            columIndices.push_back(j);
            if (0 < i) {
                rowIndices.push_back(i - 1);
            }
            if (i + 1 < puzzle.size()) {
                rowIndices.push_back(i + 1);
            }
            if (0 < j) {
                columIndices.push_back(j - 1);
            }
            if (j + 1 < puzzle[i].size()) {
                columIndices.push_back(j + 1);
            }

            for (auto outer : rowIndices) {
                for (auto inner : columIndices) {
                    if (outer == i && inner == j) {
                        continue;
                    }
                    innerCounter += puzzle[outer][inner];
                }
            }

            if (innerCounter < 4) {
                counter++;
                puzzle[i][j] = 0;
            }
        }
    }
    return counter;
}

int main() {
    ifstream puzzleFile("puzzle.txt");
    string str;

    vector<vector<int>> puzzle;
    while (std::getline(puzzleFile, str)) {
        vector<int> row;

        for (int i = 0; i < str.size(); i++) {
            if (str.at(i) == '@') {
                row.push_back(1);
            } else {
                row.push_back(0);
            }
        }
        puzzle.push_back(row);
    }
    long counter = 0;
    long newCounter = 0;
    do {
        counter = newCounter;
        newCounter = counter + removePaper(puzzle);

    } while (counter != newCounter);
    cout << counter << std::endl;
}

