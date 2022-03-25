#include "WordChain.h"

void distinctInitial(ofstream *out) {
    static int ans[26];
    static int last[26];
    if (hasRing()) {
        printf("has ring!\n");
        return;
    }
    queue<int> q;
    static int inDegree[26];
    getInDegree(inDegree);
    for (int i = 0; i < 26; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
        last[i] = -1;
    }
    while (!q.empty()) {
        int front = q.front();
        q.pop();
        FOR_ALPHA(i) {
            if (i != front && edge[front][i]) {
                inDegree[i]--;
                if (ans[i] <= ans[front]) {
                    ans[i] = ans[front] + 1;
                    last[i] = front;
                }
                if (inDegree[i] == 0) {
                    q.push(i);
                }
            }
        }
    }
    stack<int> stk;
    int mx = 0, final;
    FOR_ALPHA(i) {
        if (ans[i] > mx) {
            mx = ans[i];
            final = i;
        }
    }
    if (mx == 0) {
        FOR_ALPHA(i) {
            if (edge[i][i]) {
                *out << word[i][i][0] << endl;
                cout << word[i][i][0] << endl;
                return;
            }
        }
        return;
    }
    while (final >= 0) {
        stk.push(final);
        final = last[final];
    }
    final = stk.top();
    stk.pop();
    while (!stk.empty()) {
        *out << word[final][stk.top()][0] << endl;
        cout << word[final][stk.top()][0] << endl;
        final = stk.top();
        stk.pop();
    }
    return;
}