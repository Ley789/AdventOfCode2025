#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>
#include <cmath>
#include <functional>
#include <unordered_set>
using namespace std;


template <class T>
int positivNumDigits(T number) {
    int digits = 0;
    while (number > 0) {
        number /= 10;
        digits++;
    }
    return digits;
}

struct invalidIdGenerator;
namespace std {
    template<>
    struct hash<invalidIdGenerator>
    {
        size_t operator()(const invalidIdGenerator& c) const noexcept;
    };
}

struct invalidIdGenerator {
    const long long digitDivisor;
    const long long digitMultiplier;

    static std::unordered_set<invalidIdGenerator> computeGenerators(int digits) {
        std::unordered_set<invalidIdGenerator> ret;
        for (int i = 1; i < digits; i++) {
            if (digits % i == 0) {
                ret.insert(invalidIdGenerator(i, digits / i));
            }
        }
        return ret;
    }

    bool operator==(const invalidIdGenerator& other) const {
        return digitDivisor == other.digitDivisor && digitMultiplier == other.digitMultiplier;
    }
private:
    invalidIdGenerator(long long d, long long m) : digitDivisor(d), digitMultiplier(m) {}    
};

namespace std {
    inline size_t hash<invalidIdGenerator>::operator()(
        const invalidIdGenerator& c) const noexcept
    {
        size_t h1 = std::hash<long long>()(c.digitDivisor);
        size_t h2 = std::hash<long long>()(c.digitMultiplier);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
}


struct range {
    const long long begin;
    const long long end;

    range(long long b, long long e) : begin(b), end(e) {}

    std::unordered_set<long long> computeInvalidIdsPart2() const {
        std::unordered_set<long long> result;
        int beginDigits = positivNumDigits(begin);
        int digitRange = positivNumDigits(end) - beginDigits;
        long long val = begin;

        for (int i = 0; i <= digitRange; i++) {
            auto gen = invalidIdGenerator::computeGenerators(beginDigits + i);
    
            for (auto g : gen) {
                auto r = invalidIdsForGenerator(val, g);
                result.insert(r.begin(), r.end());
            }
            val = pow(10, beginDigits + i);
        }
        return  result;
    }

    std::unordered_set<long long> invalidIdsForGenerator(long long val, const invalidIdGenerator& gen) const {
        std::unordered_set<long long> result;
        if (end < val) { return result; }
        int digits = positivNumDigits(val);
        //sanity check, if generator is invalid --> return nothing
        if (gen.digitDivisor * gen.digitMultiplier != digits) { return result; }
        long long shift = pow(10, digits - gen.digitDivisor);
        long long seq = val / shift;
        int seqDigits = positivNumDigits(seq);
        do {
            val = concatSeq(seq, gen.digitMultiplier - 1);
            if (begin <= val && val <= end) {
                result.insert(val);
            }
            //we would increase the number of digits of the generated value --> hence we are done
            if (seqDigits != positivNumDigits(seq + 1)) {
                return result;
            }
            seq++;
        } while (val <= end);
        return result;
    }

    long long concatSeq(long long seq, int numberOfConcats) const {
        int digits = positivNumDigits(seq);
        long long shift = pow(10, digits);
        long long result = seq;
        for (int i = 0; i < numberOfConcats; i++) {
            result = result * shift + seq;
        }
        return result;
    }

    /*
        IDEA an invalid ID has structure xyz|xyz
        so first check for number of digits (must be even or it cannot be an ivalid id)
        check for invalid ID, if so store
        generate next invalid ID by xy(z+1)|xy(z+1)
        check if xy(z+1) increased the number of digits, if so jumpb by 2 digits and repeat
    */
    vector<long long> computeInvalidIds() const {
        vector<long long> ret;
        long long currentVal = begin;
        while (currentVal <= end) {
            int digits = positivNumDigits(currentVal);
            //Invalid id --> even number of digits, hence we go to the next highest number having even digits
            if (digits % 2 != 0) {
                currentVal = pow(10, digits);
                continue;
            }

            if (checkId(currentVal, digits)) {
                ret.push_back(currentVal);
            }

            long split = pow(10, digits / 2);
            long long part = currentVal / split;
            //set currentVal for the next possible invalid ID
            if (currentVal - (part * split) >= part) {
                part++;
            }
            if (positivNumDigits(part) != digits / 2) {
                currentVal = pow(10, digits + 2);
            }
            else {
                currentVal = (part * split) + part;
            }
        }
        return ret;
    }

private:
    bool checkId(long long value, int digits) const {
        if (digits % 2 != 0) {
            return true;
        }
        long power = pow(10, digits / 2);
        long long firstPart = value / power;
        return (value - firstPart * power) == firstPart;
    }
};

int main() {
    ifstream puzzleFile("puzzle.txt");
    string str;
    vector<range> puzzle;
    std::getline(puzzleFile, str);
    while (!str.empty()) {
        size_t i = str.find_first_of(",");
        string line = str.substr(0, i);
        size_t r = line.find_first_of("-");
        puzzle.push_back(range(stoll(line.substr(0, r)), stoll(line.substr(r + 1))));
        if (i == string::npos) {
            break;
        }
        str.erase(0, i + 1);
    }
 
    long long result = 0;

    for (auto r : puzzle) {
        auto ids = r.computeInvalidIds();
        result += std::reduce(ids.begin(), ids.end(), 0LL, std::plus<long long>{});     
    }

    cout << result << endl;

    result = 0;
    for (auto r : puzzle) {
        auto ids = r.computeInvalidIdsPart2();
        result += std::reduce(ids.begin(), ids.end(), 0LL, std::plus<long long>{});
    }

    cout << result << endl;
}


