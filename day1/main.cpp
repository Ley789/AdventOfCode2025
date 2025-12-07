#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


int main() {
    ifstream puzzleFile("puzzle.txt");
    string str;
    int position = 50;
    int base = 100;

    long long resultPart1 = 0;
    long long resultPart2 = 0;
 
    while (std::getline(puzzleFile, str)) {

        if (str.empty()) {
            continue;
        }
        char rotation = str[0];
        int howMuch = stoi(str.substr(1));

        if (howMuch == 0) { continue; }

        if (rotation == 'R') {
            position += howMuch;
        }
        else {
            position -= howMuch;
        }

        int divByBase = abs(position / base);

        if (rotation == 'R') {
            resultPart2 += divByBase;
        }
        else if (position <= 0) {
            resultPart2 += divByBase + 1;
            if (position + howMuch == 0) {
                resultPart2--;
            }
        }

        position = (((divByBase + 1) * base + position) % base);
        if (position == 0) {
            resultPart1++;
        }
    }
    cout << position << endl;


    cout << resultPart1 << endl;
    cout << resultPart2 << endl;
}




