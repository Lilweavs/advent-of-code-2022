
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <unordered_set>
#include <cmath>

#include "clock.h"

using namespace std;

Timer timer;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct Location {
    Location(int a, int b) : x(a), y(b) { };
    Location() { };
    int x = 0;
    int y = 0;
};

bool operator==(const Location &lhs, const Location &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Location operator-(const Location &lhs, const Location &rhs) {
    return Location(lhs.x - rhs.x, lhs.y - rhs.y);
}

struct struct_hash
{
    std::size_t operator() (const Location &L) const
    {
        return hash<int>{}(L.x) ^ hash<int>{}(L.y);
    }
};

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<pair<char,int>> simulation;
    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {
        pair<char, int> tmp(line[0], stoi(string(line.begin()+2,line.end())));
        simulation.push_back(tmp);
    }
    ifile.close();

    unordered_set<Location, struct_hash> visited;

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    /* Part 1 */
    timer.tick();
    Location head(0,0);
    Location tail(0,0);
    visited.insert(tail);
    for (const auto& sim : simulation) {
        switch (sim.first)
        {
        case 'R':
            head.x += sim.second;

            for (int i = 0; i < sim.second; i++) {
                Location diff = head - tail;

                if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                    continue; 
                } else {
                    if (diff.y != 0)
                        tail.y += sgn(diff.y);
                    tail.x += sgn(diff.x);
                    visited.insert(tail);
                }
            }
            break;
        case 'L':
            head.x -= sim.second;

            for (int i = 0; i < sim.second; i++) {
                Location diff = head - tail;

                if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                    continue; 
                } else {
                    if (diff.y != 0)
                        tail.y += sgn(diff.y);
                    tail.x += sgn(diff.x);
                    visited.insert(tail);
                }
            }
            break;

        case 'U':
            head.y += sim.second;

            for (int i = 0; i < sim.second; i++) {
                Location diff = head - tail;

                if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                    continue; 
                } else {
                    if (diff.x != 0)
                        tail.x += sgn(diff.x);
                    tail.y += sgn(diff.y);
                    visited.insert(tail);
                }
            }
            break;

        case 'D':
            head.y -= sim.second;

            for (int i = 0; i < sim.second; i++) {
                Location diff = head - tail;

                if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                    continue; 
                } else {
                    if (diff.x != 0)
                        tail.x += sgn(diff.x);
                    tail.y += sgn(diff.y);
                    visited.insert(tail);
                }
            }
            break;
        }
    }

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << visited.size() << endl;

    /* Part 2 */
    timer.tick();

    array<Location, 10> rope;

    visited.clear();
    visited.insert(Location(0,0));
    for (const auto& sim : simulation) {
        
        for (int k = 0; k < sim.second; k++) {
            if (sim.first == 'R')
                rope[0].x++;
            else if (sim.first == 'L')
                rope[0].x--;
            else if (sim.first == 'U')
                rope[0].y++;
            else
                rope[0].y--;

            for (size_t j = 0; j < rope.size()-1; j++){
                Location head = rope[j];
                Location tail = rope[j+1];

                Location diff = head - tail;            
                
                char cmd = '\0';
                int  mx = 0;
                if (diff.x > 1) {
                    cmd = 'R';
                    mx = diff.x;
                } else if (diff.x < -1) {
                    cmd = 'L';
                    mx = -1*diff.x;
                } else if (diff.y > 1) {
                    cmd = 'U';
                    mx = diff.y;
                } else if (diff.y < -1) {
                    cmd = 'D';
                    mx = -1*diff.y;
                } else {
                    cmd = '\0';
                }

                switch (cmd)
                {
                case 'R':
                    // head.x += sim.second;

                    for (int i = 0; i < mx; i++) {
                        Location diff = head - tail;

                        if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                            continue; 
                        } else {
                            if (diff.y != 0)
                                tail.y += sgn(diff.y);
                            tail.x += sgn(diff.x);
                            if (j == 8)
                                visited.insert(tail);
                        }
                    }
                    break;
                case 'L':
                    // head.x -= sim.second;

                    for (int i = 0; i < mx; i++) {
                        Location diff = head - tail;

                        if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                            continue; 
                        } else {
                            if (diff.y != 0)
                                tail.y += sgn(diff.y);
                            tail.x += sgn(diff.x);
                            if (j == 8)
                                visited.insert(tail);
                        }
                    }
                    break;

                case 'U':
                    // head.y += sim.second;

                    for (int i = 0; i < mx; i++) {
                        Location diff = head - tail;

                        if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                            continue; 
                        } else {
                            if (diff.x != 0)
                                tail.x += sgn(diff.x);
                            tail.y += sgn(diff.y);
                            if (j == 8)
                                visited.insert(tail);
                        }
                    }
                    break;
                    
                case 'D':
                    // head.y -= sim.second;

                    for (int i = 0; i < mx; i++) {
                        Location diff = head - tail;

                        if ((diff.x <= 1 && diff.x >= -1) && (diff.y <= 1 && diff.y >= -1)) { 
                            continue; 
                        } else {
                            if (diff.x != 0)
                                tail.x += sgn(diff.x);
                            tail.y += sgn(diff.y);
                            if (j == 8)
                                visited.insert(tail);
                        }
                    }
                    break;
                default:
                    break;
                }
                rope[j+1] = tail;
            }
        }
    }

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << visited.size() << endl;

    return 0;
}   