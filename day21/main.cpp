#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <unordered_map>
#include <cmath>

#include "clock.h"

using namespace std;

Timer timer;

struct Monkey {
    string leftMonkey  = "";
    string rightMonkey = "";
    string instruction;
    double value = 0;
};

void CopyDictOne(const unordered_map<string, Monkey>& left, unordered_map<string, Monkey>& right) {
    for (const auto& key : left) {
        right.insert(key);
    }
}

void CopyDictTwo(const unordered_map<string, double>& left, unordered_map<string, double>& right) {
    for (const auto& key : left) {
        right.insert(key);
    }
}


void SolveMonkey(Monkey& monkey, const string& name, unordered_map<string, Monkey>& unSolved, unordered_map<string, double>& solved) {

    if (!solved.contains(monkey.leftMonkey)) {
        SolveMonkey(unSolved.at(monkey.leftMonkey), monkey.leftMonkey, unSolved, solved);
    }

    if (!solved.contains(monkey.rightMonkey)) {
        SolveMonkey(unSolved.at(monkey.rightMonkey), monkey.rightMonkey, unSolved, solved);
    }

    if (monkey.instruction == "+")
        monkey.value = solved.at(monkey.leftMonkey) + solved.at(monkey.rightMonkey);
    else if (monkey.instruction == "-")
        monkey.value = solved.at(monkey.leftMonkey) - solved.at(monkey.rightMonkey);
    else if (monkey.instruction == "*")
        monkey.value = solved.at(monkey.leftMonkey) * solved.at(monkey.rightMonkey);
    else
        monkey.value = solved.at(monkey.leftMonkey) / solved.at(monkey.rightMonkey);

    solved.insert(pair(name, monkey.value));
    unSolved.erase(name);
}

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<array<int,3>> instructions;
    array<string,9> cargo;


    unordered_map<string, Monkey> unSolved;
    unordered_map<string, double> solved;

    regex rx("([a-zA-Z]+): (([a-zA-Z]+) ([+-\\/*]) ([a-zA-Z]+)|(\\d+))");
    ifstream ifile("input.txt", std::ios::in);
    while (getline(ifile, line, '\n')) {

        smatch match;
        regex_search(line, match, rx);

        Monkey monkey;

        if (match[3].matched) {
            monkey.leftMonkey  = match[3].str();
            monkey.instruction = match[4].str();
            monkey.rightMonkey = match[5].str();

            unSolved.insert(pair(match[1].str(), monkey));
        } else {
            solved.insert(pair(match[1].str(), stod(match[2].str())));
        }

    }
    ifile.close();


    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    unordered_map<string, Monkey> tmpUnSolved;
    unordered_map<string, double> tmpSolved;
    timer.tick();


    CopyDictOne(unSolved, tmpUnSolved);
    CopyDictTwo(solved, tmpSolved);


    timer.tock();
    SolveMonkey(tmpUnSolved.at("root"), "root", tmpUnSolved, tmpSolved);

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << (uint64_t)tmpSolved.at("root") << endl;

    timer.tick();
    string leftMonkey  = unSolved.at("root").leftMonkey;
    string rightMonkey = unSolved.at("root").rightMonkey;

    double ridx = UINT64_MAX;
    double lidx = 0;
    double cidx = 0;
    
    do {
    
    cidx = (lidx + ridx) / 2;

    tmpUnSolved.clear();
    tmpSolved.clear();

    CopyDictOne(unSolved, tmpUnSolved);
    CopyDictTwo(solved, tmpSolved);

    tmpSolved.at("humn") = cidx;

    SolveMonkey(tmpUnSolved.at("root"), "root", tmpUnSolved, tmpSolved);

    if (tmpSolved.at(leftMonkey) > tmpSolved.at(rightMonkey))
        lidx = cidx;
    else
        ridx = cidx;

    } while (fabs(tmpSolved.at(leftMonkey) - tmpSolved.at(rightMonkey)) > 1);

    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 2: " << (uint64_t)cidx << endl;

    return 0;
}