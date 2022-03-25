#ifndef _WORDCHAIN_H_
#define _WORDCHAIN_H_
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <cassert>
#include <set>
using namespace std;

extern vector<string> word[26][26];
extern int edge[26][26];

void countAndList(ofstream*);
void maxWord(ofstream*);
void distinctInitial(ofstream*);
bool hasRing();
void getInDegree(int*);

#define FOR_ALPHA(x) for(int x = 0; x < 26; x++)

#define DEBUG false
#define DEBEG_REACHED \
    if (DEBUG) { \
        cout << __FILE__ << " " << __LINE__ << endl; \
    }

typedef unsigned long long u64;

#endif