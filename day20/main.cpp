#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>

#include "clock.h"

using namespace std;

struct List {
    List(int64_t a) : value(a) { };
    List() { };
    List* left;
    List* right;
    int64_t value;
};

int64_t findEncryptionNumber(List* l, int numAfter) {

    List* ptr = l;
    for (int i = 0; i < numAfter; i++)
        ptr = ptr->right;

    return ptr->value;
};

Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();

    string line;
    // vector<int64_t> sequence;
    // vector<int64_t> mixedSequence;

    vector<List*> shuffleOrder;
    List* listMiddle = new List(0);

    ifstream ifile("input.txt", std::ios::in);
    int i = 0;
    while (getline(ifile, line)) {
        int64_t value = stoll(line);
        shuffleOrder.push_back(new List(value));
        if (value == 0) { listMiddle = shuffleOrder.back(); }
        if (i) {
            shuffleOrder[i]->left = shuffleOrder[i-1];
            shuffleOrder[i-1]->right = shuffleOrder[i]; 
        }
        i++;
    }
    shuffleOrder.front()->left = shuffleOrder.back();
    shuffleOrder.back()->right = shuffleOrder.front();

    ifile.close();

    cout << "Input Parsing: " << timer.tock() / 1000.0 << "ms" << endl;

    timer.tick();
    for (const auto& num : shuffleOrder){

        int shuffleCount = num->value;

        if (shuffleCount >= 0) {
            for (int i = 0; i < shuffleCount; i++) {
                /* 1 2 -3 4 for 1*/
                List* lNum = num->left;           // lnum  = 4
                List* rNum = num->right;          // rnum  = 2
                List* rrNum = num->right->right;  // rrnum = -3

                rrNum->left = num;
                rNum->right = num;

                rNum->left  = lNum; // same same
                lNum->right = rNum; // same same

                num->left  = rNum;
                num->right = rrNum;
            }
        } else {
            for (int i = 0; i < -shuffleCount; i++) {
                /* 1 -1 2 4 for -1*/
                List* lNum = num->left;           // lnum  = 1
                List* rNum = num->right;          // rnum  = 2
                List* llNum = num->left->left;    // llNum = 4

                llNum->right = num;
                lNum->left   = num;

                lNum->right = rNum; // same same
                rNum->left  = lNum; // same same

                num->left  = llNum;
                num->right = lNum;
            }
        }
    }

    // List* ptr = shuffleOrder.front()->right;
    // do {
    //     cout << ptr->value << " " << endl;
    //     ptr = ptr->right;
    // } while (ptr != shuffleOrder.front()->right);

    int64_t sum = 0;
    for (int i = 1; i < 4; i++)
        sum += findEncryptionNumber(listMiddle, 1000*i);

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << endl;
    cout << "Part 1: " << sum << endl;


    for (auto& ptr : shuffleOrder)
        delete ptr;
    shuffleOrder.clear();

    ifile.open("input.txt", std::ios::in);
    i = 0;
    while (getline(ifile, line)) {
        int64_t value = stoll(line) * 811589153;
        shuffleOrder.push_back(new List(value));
        if (value == 0) { listMiddle = shuffleOrder.back(); }
        if (i) {
            shuffleOrder[i]->left = shuffleOrder[i-1];
            shuffleOrder[i-1]->right = shuffleOrder[i]; 
        }
        i++;
    }
    shuffleOrder.front()->left = shuffleOrder.back();
    shuffleOrder.back()->right = shuffleOrder.front();

    ifile.close();

    timer.tick();
    for (int k = 0; k < 10; k++) {
        for (const auto& num : shuffleOrder){

            int shuffleCount = (num->value % static_cast<int64_t>(shuffleOrder.size() - 1));

            if (shuffleCount >= 0) {
                for (int i = 0; i < shuffleCount; i++) {
                    /* 1 2 -3 4 for 1*/
                    List* lNum = num->left;           // lnum  = 4
                    List* rNum = num->right;          // rnum  = 2
                    List* rrNum = num->right->right;  // rrnum = -3

                    rrNum->left = num;
                    rNum->right = num;

                    rNum->left  = lNum; // same same
                    lNum->right = rNum; // same same

                    num->left  = rNum;
                    num->right = rrNum;
                }
            } else {
                for (int i = 0; i < -shuffleCount; i++) {
                    /* 1 -1 2 4 for -1*/
                    List* lNum = num->left;           // lnum  = 1
                    List* rNum = num->right;          // rnum  = 2
                    List* llNum = num->left->left;    // llNum = 4

                    llNum->right = num;
                    lNum->left   = num;

                    lNum->right = rNum; // same same
                    rNum->left  = lNum; // same same

                    num->left  = llNum;
                    num->right = lNum;
                }
            }
        }
    }

    sum = 0;
    for (int i = 1; i < 4; i++)
        sum += findEncryptionNumber(listMiddle, 1000*i);


    cout << "Part 2 Elapsed: " << timer.tock() / 1000.0 << endl;
    cout << "Part 2: " << sum << endl;

    for (auto& ptr : shuffleOrder)
        delete ptr;
    shuffleOrder.clear();

    return 0;
}   