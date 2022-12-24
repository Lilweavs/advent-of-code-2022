#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>

#include "clock.h"

using namespace std;

struct List {
    List(int a) : value(a) { };
    List* left;
    List* right;
    int value;
};


int findEncryptionNumber(List* l, int numAfter) {

    List* ptr = l;
    for (int i = 0; i < numAfter; i++)
        ptr = ptr->right;

    cout << ptr->value << endl;;
    return ptr->value;
}


Timer timer;

int main(int argc, char* argv[]) {

    timer.tick();

    string line;
    vector<int> sequence;
    vector<int> mixedSequence;

    vector<List*> shuffleOrder;
    List* listMiddle;

    ifstream ifile("input.txt", std::ios::in);
    // while (getline(ifile, line)) { sequence.push_back(stoi(line)); }
    int i = 0;
    while (getline(ifile, line)) {
        int value = stoi(line);
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


    // while (ptr != shuffleOrder.front()) {
    //     cout << ptr->value << " " << endl;
    //     ptr = ptr->right;
    // }

    
    // cout << findEncryptionNumber(listMiddle, 1000) << endl;
    // cout << findEncryptionNumber(listMiddle, 2000) << endl;
    // cout << findEncryptionNumber(listMiddle, 3000) << endl;
    int sum = 0;
    for (int i = 1; i < 4; i++)
        sum += findEncryptionNumber(listMiddle, 1000*i);

    cout << "Part 1 Elapsed: " << timer.tock() / 1000.0 << endl;
    cout << "Part 1: " << sum << endl;



    for (auto& ptr : shuffleOrder)
        delete ptr;
    shuffleOrder.clear();


    // mixedSequence = sequence;

    // int mod = sequence.size();
    // for (size_t i = 0; i < sequence.size(); i++) {
    //     int tmp = sequence[i];

    //     auto it = find(mixedSequence.begin(), mixedSequence.end(), tmp);
    //     int index = distance(mixedSequence.begin(), it);
        
    //     if (tmp < 0) {
    //         // go backwards
            
    //         int l = (index + tmp) % mod ;
            
    //         if (l <= 0) {
    //             for (int j = index; j < mod + l - 1 ; j++) {
    //                 swap(mixedSequence[j], mixedSequence[j+1]);
    //             }
    //         } else {
    //             for (int j = index; j > l ; j--) {
    //                 swap(mixedSequence[j], mixedSequence[j-1]);
    //             }
    //         }            
    //     } else {
    //         int l = (index + tmp) % mod ;
            
    //         if (l < tmp) {
    //             for (int j = index; j > l + 1 ; j--) {
    //                 swap(mixedSequence[j], mixedSequence[j-1]);
    //             }
    //         } else {
    //             for (int j = index; j < l ; j++) {
    //                 swap(mixedSequence[j], mixedSequence[j+1]);
    //             }
    //         }          
    //     }
    // }

    // auto it = find(mixedSequence.begin(), mixedSequence.end(), 0);
    // int zeroindex = distance(mixedSequence.begin(), it);
    
    // int ans = mixedSequence[(zeroindex + 1000) % mod] + mixedSequence[(zeroindex + 2000) % mod] + mixedSequence[(zeroindex + 3000) % mod];
    // cout << mixedSequence[(zeroindex + 1000) % mod] << endl;
    // cout << mixedSequence[(zeroindex + 2000) % mod] << endl;
    // cout << mixedSequence[(zeroindex + 3000) % mod] << endl;

    // cout << "Time Elapsed: " << timer.tock() / 1000.0 << "ms" << endl;
    // cout << "Part 1: " << ans << endl;

    return 0;
}   