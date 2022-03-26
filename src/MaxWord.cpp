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
    FOR_ALPHA(i) {
        dp[i] = -10000;
    }
    int last[26];
    memset(last, 0, sizeof(last));
    FOR_ALPHA(i) {
        if (inDegreee[i] == 0) {
            q.push(i);
            dp[i] = edge[i][i];
        }
        last[i] = -1;
    }
    while (!q.empty()) {
        int front = q.front();
        q.pop();
        FOR_ALPHA(i) {
            if (i != front && edge[front][i]) {
                if (1 + edge[i][i] + dp[front] > dp[i]) {
                    dp[i] = 1 + edge[i][i] + dp[front];
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
        *out << word[final][stk.top()][0] << endl;
        cout << word[final][stk.top()][0] << endl;
        final = stk.top();
        stk.pop();
    }
}

void maxWord(ofstream *out) {
    if (hasRing()) {
        cout << "has ring" << endl;
        return;
    }
    noRingHandler(out);
    return;
}