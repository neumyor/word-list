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
#include <sstream>

using namespace std;

extern vector<string> word[26][26];
extern int edge[26][26];
extern char headLetter, tailLetter;
extern bool allowRing;
extern int sccBelong[26];
extern bool sccEdge[26][26];
extern int sccInDegree[26];
extern int sccNumber;
extern int sccOutDegree[26];
extern vector<int> sccElement[26];
extern set<int> sccInFromStart[26];
extern set<int> sccInFromFinish[26];

void countAndList(ofstream*);
void maxWord(ofstream*);
void distinctInitial(ofstream*);
void maximizeLetter(ofstream*);
bool hasRing();
void getInDegree(int*);
void headLetterInDegree(int*);
void calcScc();
void bfsFromStart(int);
void bfsFromFinish(int);

#define FOR_ALPHA(x) for(int x = 0; x < 26; x++)
#define FOR_SCC(x) for(int x = 0; x < sccNumber; x++)

#define DEBUG false
#define DEBEG_REACHED \
    if (DEBUG) { \
        cout << __FILE__ << " " << __LINE__ << endl; \
    }

typedef unsigned long long u64;

#endif