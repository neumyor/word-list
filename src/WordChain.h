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

enum class HandlerType {
    COUNT_AND_LIST,
    DISTINCT_INITIAL,
    MAX_LETTER,
    MAX_WORD,
    UNKNOWN
};

struct StringCmp {
    bool operator() (const string *a, const string *b) const {
        return a->length() < b->length();
    }
};
typedef set<string*, StringCmp> StringSet;

#define FOR_ALPHA(x) for(int x = 0; x < 26; x++)
#define FOR_SCC(x) for(int x = 0; x < sccNumber; x++)

#define DEBUG true
#define  DEBUG_REACHED                                  \
    if (DEBUG) {                                        \
        cout << __FILE__ << " " << __LINE__ << endl;    \
    }
#define MAX_RESULT_LINE 20000
#pragma warning(disable:4251)
#endif