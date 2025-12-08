#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <cmath>
#include <vector>
#include <algorithm> 
using namespace std;


struct box {
    long id;
    double x;
    double y;
    double z;

    static double distance(const box& f, const box& s) {
        if (f.id == s.id) { return 0; }
        return sqrt(pow((f.x - s.x), 2) + pow((f.y - s.y), 2) + pow((f.z - s.z), 2));
    }

    friend ostream& operator<<(ostream&, box& c) {
        cout << "Line#: " << c.id + 1 << " X: " << c.x << " Y: " << c.y << " Z: " << c.z;
        return cout;
    }
};

struct dist {
    dist(box f, box s) : fBox(f), sBox(s) {
        if (fBox.id > sBox.id) {
            std::swap(fBox, sBox);
        }
        distance = box::distance(fBox, sBox);
    }

    bool operator<(const dist& other) const {
        if (distance < other.distance) { return true; }
        if (distance > other.distance) { return false; }
        return fBox.id < sBox.id;
    }

    bool operator=(const dist& other) const {
        return distance == other.distance && fBox.id == sBox.id;
    }

    double getDistance() const {
        return distance;
    }

    friend ostream& operator<<(ostream&, dist& c) {
        cout << "Distance: " << c.distance << " between boxes at line: " << c.fBox.id + 1 << " --- " << c.sBox.id + 1 << " X BOX1 : " << c.fBox.x << " BOX2 " << c.sBox.x;
        return cout;
    }
    long getFirstBoxId() {
        return fBox.id;
    }
    long getSecondBoxId() {
        return sBox.id;
    }

    long long getResultPart2() {
        return static_cast<long long>(fBox.x) * static_cast<long long>(sBox.x);
    }
private:
    double distance;
    box fBox;
    box sBox;
};

static inline bool hasIntersection(const set<long>& f, const set<long>& s) {
    if (f.empty()) { return false; }
    for (auto v : f) {
        if (s.contains(v)) { return true; }
    }
    return false;
}

int main() {
    ifstream puzzleFile("puzzle.txt");
    string str;
    set<dist> puzzle;
    vector<box> boxes;
    long lineCounter = 0;
    long numOfConnection = 1000;
    while (std::getline(puzzleFile, str)) {
        box b;
        b.id = lineCounter;
        size_t coordinatePos = str.find_first_of(",");

        b.x = stod(str.substr(0, coordinatePos));
        str.erase(0, coordinatePos + 1);
        coordinatePos = str.find_first_of(",");
        b.y = stod(str.substr(0, coordinatePos));
        str.erase(0, coordinatePos + 1);
        b.z = stod(str);

        for (int i = 0; i < boxes.size(); i++) {
            puzzle.insert(dist(boxes[i], b));
        }

        boxes.push_back(b);
        lineCounter++;
    }
    lineCounter = 0;
    vector<set<long>> mergeUs;
    auto it = puzzle.begin();
    for (; it != puzzle.end(); it++) {
        if (lineCounter == numOfConnection) {

            std::vector<long> sizes(mergeUs.size());
            std::transform(mergeUs.begin(), mergeUs.end(), sizes.begin(), [](const set<long>& s) { return s.size(); });
            sort(sizes.begin(), sizes.end());

            cout << sizes[sizes.size() - 1] * sizes[sizes.size() - 2] * sizes[sizes.size() - 3] << endl;
        }
        vector<set<long>> tmp;
        dist v = *it;
        set<long> connection;
        connection.insert(v.getFirstBoxId());
        connection.insert(v.getSecondBoxId());
        for (const auto& s : mergeUs) {
            if (hasIntersection(s, connection)) {
                connection.insert(s.begin(), s.end());
            } else {
                tmp.push_back(s);
            }
        }
        mergeUs = tmp;
        mergeUs.push_back(connection);
        lineCounter++;
        //part2
        if (mergeUs.size() == 1 && mergeUs[0].size() == boxes.size()) {
            cout << v.getResultPart2() << endl;
            break;
        }
    }
}