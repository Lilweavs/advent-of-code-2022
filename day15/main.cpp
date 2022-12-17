#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <cmath>
#include <unordered_set>

#include "clock.h"

using namespace std;

Timer timer;

struct Point {
    int x = 0;
    int y = 0;
};

bool operator==(const Point& lhs, const Point& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

struct struct_hash
{
    size_t operator() (const Point& p) const
    {
        return hash<int>{}(p.x) ^ hash<int>{}(p.y);
    }
};

bool reachable(const Point& lhs, const Point& rhs, int md) {
    return (abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y) <= md);
}




int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<array<int,3>> instructions;

    ifstream ifile("input.txt", std::ios::in);

    vector<array<int, 5>> sensorMeausrements;

    unordered_set<Point, struct_hash> Beacons;
    unordered_set<Point, struct_hash> Sensors;

    regex rx("[-+]?\\d+");
    while (getline(ifile, line, '\n')) {
        array<int, 5> tmp;
        sregex_iterator it(line.begin(), line.end(), rx);
        int i = 0;
        while (it != sregex_iterator()) {
            smatch match = *it++;
            tmp[i++] = stoi(match.str());
        }
        Sensors.insert(Point({tmp[0], tmp[1]}));
        Beacons.insert(Point{tmp[2], tmp[3]});
        tmp[i] = abs(tmp[0] - tmp[2]) + abs(tmp[1] - tmp[3]);
        sensorMeausrements.push_back(tmp);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;


    timer.tick();
    unordered_set<int> intersection;
    int target = 2000000;
    for (const auto& sensor : sensorMeausrements) {
        int ptx = sensor[0];
        int pty = sensor[1];
        int md = sensor[4];

        if (abs(pty - target) <= md) {
            int r = md - abs(target - pty);

            Point tmp{0,target};
            for (int i = ptx - r; i <= ptx + r; i++) {
                tmp.x = i;
                if (!Beacons.contains(tmp) && !Sensors.contains(tmp))
                    intersection.insert(i);
            }
        }
    }

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << intersection.size() << endl;
    // cout << "Part 2: " << part2(instructions, cargo) << endl;

    timer.tick();
    unordered_set<Point, struct_hash> considering;
    for (const auto& sensor : sensorMeausrements) {
        int ptx = sensor[0];
        int pty = sensor[1];
        int md = sensor[4] + 1;
        
        Point up   = {ptx,pty-md};
        Point down = {ptx,pty+md};

        for (int i = 0; i <= md; i++) {

            Point tmp = {up.x + i, up.y + i};
            
            if (!(tmp.x < 0 || tmp.x > 4000000 || tmp.y < 0 || tmp.y > 4000000)) { 
                if (!Beacons.contains(tmp) && !Sensors.contains(tmp)) {
                    considering.insert(tmp);
                }
            }

            
            tmp = {up.x - i, up.y + i};

            if (!(tmp.x < 0 || tmp.x > 4000000 || tmp.y < 0 || tmp.y > 4000000)) { 
                if (!Beacons.contains(tmp) && !Sensors.contains(tmp)) {
                    considering.insert(tmp);
                }
            }
            
            tmp = {down.x + i, down.y - i};

            if (!(tmp.x < 0 || tmp.x > 4000000 || tmp.y < 0 || tmp.y > 4000000)) { 
                if (!Beacons.contains(tmp) && !Sensors.contains(tmp)) {
                    considering.insert(tmp);
                }
            }
            
            tmp = {down.x - i, down.y - i};

            if (!(tmp.x < 0 || tmp.x > 4000000 || tmp.y < 0 || tmp.y > 4000000)) { 
                if (!Beacons.contains(tmp) && !Sensors.contains(tmp)) {
                    considering.insert(tmp);
                }
            }
            
        }

    }

    cout << considering.size() << endl;
    Point found;

    // cout << considering.contains(Point{14,11}) << endl;

    for (const auto& p : considering) {
        bool f = false;  
        for (const auto& s : sensorMeausrements) {
            Point tmp = {s[0], s[1]};
            
            f = reachable(p,tmp,s[4]);
            if (f) { break; }
        }
        if (!f) {
            found = p;
        }
    }

    uint64_t ans = 0;
    // cout << found.x << ", " << found.y << endl;
    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << found.x << ", " << found.y << endl;
    // cout << "Part 2: " << 4000000 * found.x + found.y << endl;
    ans = (uint64_t)(4000000) * (uint64_t)(found.x) + (uint64_t)(found.y);
    cout << "Part 2: " << ans << endl;
    // for (int i = 0; i <= 4000000; i++) {
    //     for (int j = 0; j <= 4000000; j++) {
    //         Point tmp = {i, j};
    //         if (!intersection.contains(tmp)) {
    //             cout << i << ", " << j << endl;
    //         }
    //     }
    // }


    return 0;
}