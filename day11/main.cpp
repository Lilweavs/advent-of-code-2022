#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <regex>
#include <deque>

#include "clock.h"

using namespace std;

enum class Commands {
    add,
    mult,
    selfAdd,
    selfMult,
    none
};

struct Monkey {
    deque<uint64_t> items;
    array<int,2> throw2Monkey = {0};
    uint64_t divisor = 1;
    pair<Commands, uint64_t> operation = {Commands::none, 0};
    int cnt = 0;
};

bool isDivisible(uint64_t item, uint64_t div) {
    bool tmp = false;
    if (item % div == 0)
        tmp = true;
    return tmp;
}

Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();
    string line;
    vector<Monkey> monkeys;
    ifstream ifile("input.txt", std::ios::in);
    // regex rx("([\\*|\\+] )?\\d+");
    regex rx("\\d+");
        
    while (getline(ifile, line, '\n')) {

        Monkey monkey;
        while(!line.empty()) {
            getline(ifile, line, '\n');

            /* Parse Starting Items: */
            sregex_iterator it(line.begin(), line.end(), rx);
            while (it != sregex_iterator()) {
                smatch match = *it++;
                monkey.items.push_back(stoi(match.str()));   
            }

            /* Parse Operation: */
            getline(ifile, line, '\n');
            if (line.ends_with("old")) {
                if (line.find('+') != string::npos) {
                    monkey.operation.first = Commands::selfAdd;
                } else {
                    monkey.operation.first = Commands::selfMult;
                }
            } else {
                int pos = line.rfind(" ");
                if (line.find('+') != string::npos) {
                    monkey.operation.first = Commands::add;
                    monkey.operation.second = stoi(string(line.begin()+pos, line.end()));
                } else {
                    monkey.operation.first = Commands::mult;
                    monkey.operation.second = stoi(string(line.begin()+pos, line.end()));
                }
            }

            /* Parse Test: */
            getline(ifile, line, '\n');
            int pos = line.rfind(" ");
            monkey.divisor = stoi(string(line.begin()+pos, line.end()));
            for (int i = 0; i < 2; i++) {
                getline(ifile, line, '\n');
                int pos = line.rfind(" ");
                monkey.throw2Monkey[i] = stoi(string(line.begin()+pos, line.end()));
            }
            if (!getline(ifile,line,'\n')) { break; };
        }
        monkeys.push_back(monkey);
    }
    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    for (int i = 0; i < 20; i++) {
        for (auto& monkey : monkeys) {

            while (!monkey.items.empty()) {
                uint64_t item = monkey.items.front(); 
                monkey.items.pop_front();
                monkey.cnt++;

                if (monkey.operation.first == Commands::add) {
                    item = (item + monkey.operation.second);
                } else if (monkey.operation.first == Commands::mult) {
                    item = (item * monkey.operation.second);
                } else if (monkey.operation.first == Commands::selfAdd) {
                    item = (item + item);
                } else {
                    item = (item * item);
                }

                item /= 3;

                bool check = isDivisible(item, monkey.divisor);
                int whichMonkey = 0;
                if (check == true)
                    whichMonkey = monkey.throw2Monkey[0];
                else
                    whichMonkey = monkey.throw2Monkey[1];

                monkeys[whichMonkey].items.push_back(item);
            }
        }
    }
    
    vector<uint64_t> inspections;
    for (const auto& monkey : monkeys)
        inspections.push_back(monkey.cnt);

    sort(inspections.rbegin(), inspections.rend()); 

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    cout << "Part 1: " << inspections[0]*inspections[1] << endl;

    

    // cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    // cout << "Part 2: " << overlaps << endl;

    return 0;
}   