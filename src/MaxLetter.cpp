#include "WordChain.h"

static void noRingHandler(ofstream *out) {
    int inDegreee[26];
    if (headLetter) {
        headLetterInDegree(inDegreee);
    } else {
        getInDegree(inDegreee);
    }
    queue<int> q;
    int dp[26];
    int selfLength[26];
    FOR_ALPHA(i) {
        if (edge[i][i]) {
            selfLength[i] = word[i][i].back().length();
        } else {
            selfLength[i] = 0;
        }
    }
    memset(dp, 0, sizeof(dp));
    int last[26];
    FOR_ALPHA(i) {
        if (inDegreee[i] == 0) {
            q.push(i);
            dp[i] = selfLength[i];
        }
        last[i] = -1;
    }
    while (!q.empty()) {
        int front = q.front();
        q.pop();
        FOR_ALPHA(i) {
            if (i != front && edge[front][i]) {
                if (word[front][i].back().length() + selfLength[i] + dp[front] > dp[i]) {
                    dp[i] = word[front][i].back().length() + selfLength[i] + dp[front];
                    last[i] = front;
                }
                inDegreee[i]--;
                if (inDegreee[i] == 0) {
                    q.push(i);
                }
            }
        }
    }
    int final;
    if (tailLetter) {
        final = tailLetter - 'a';
    } else {
        int mx = 0;
        FOR_ALPHA(i) {
            if (dp[i] > mx) {
                mx = dp[i];
                final = i;
            }
        }
    }
    stack<int> stk;
    while (final >= 0) {
        stk.push(final);
        if (edge[final][final]) {
            stk.push(final);
        }
        final = last[final];
    }
    if (stk.size() < 3) {
        cout << "no chain" << endl;
        *out << "no chain" << endl;
        return;
    }
    final = stk.top();
    stk.pop();
    while (!stk.empty()) {
        *out << word[final][stk.top()].back() << endl;
        cout << word[final][stk.top()].back() << endl;
        final = stk.top();
        stk.pop();
    }
}

void maximizeLetter(ofstream *out) {
    if (hasRing()) {
        cout << "has ring" << endl;
        return;
    }
    noRingHandler(out);
    return;
}