#include "WordChain.h"

static u64 count() {
    static u64 dp[26];
    static int inDegree[26];
    queue<int> q;
    getInDegree(inDegree);
    FOR_ALPHA(i) {
        dp[i] = 1 + edge[i][i];
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int front = q.front();
        q.pop();
        FOR_ALPHA(i) {
            if (i != front && edge[front][i]) {
                inDegree[i]--;
                dp[i] += (1 + edge[i][i]) * dp[front] * edge[front][i];
                if (inDegree[i] == 0) {
                    q.push(i);
                }
            }
        }
    }
    u64 res = 0;
    FOR_ALPHA(i) {
        res += dp[i] - 1;
        FOR_ALPHA(j) {
            res -= edge[i][j];
        }
    }
    return res;
}

u64 totChain;
string *previous[100];
ofstream *out;
static void dfs(int cur, int len) {
    FOR_ALPHA(i) {
        if (i != cur && edge[cur][i]) {
            for (int j = word[cur][i].size() - 1; j >= 0; j--) {
                previous[len + 1] = &word[cur][i][j];
                if (len >= 1) {
                    for (int k = 1; k <= len + 1; k++) {
                        *out << *previous[k] << " ";
                        cout << *previous[k] << " ";
                    }
                    totChain--;
                    *out << endl;
                    cout << endl;
                }
                dfs(i, len + 1);
            }
        }
    }
    if (edge[cur][cur]) {
        len++;
        previous[len] = &word[cur][cur][0];
        if (len >= 2) {
            for (int k = 1; k <= len; k++) {
                    *out << *previous[k] << " ";
                    cout << *previous[k] << " ";
            }
            totChain--;
            *out << endl;
            cout << endl;
        }
        FOR_ALPHA(i) {
            if (i != cur && edge[cur][i]) {
                for (int j = word[cur][i].size() - 1; j >= 0; j--) {
                    previous[len + 1] = &word[cur][i][j];
                        if (len >= 1) {
                            for (int k = 1; k <= len + 1; k++) {
                                *out << *previous[k] << " ";
                                cout << *previous[k] << " ";
                            }
                        totChain--;
                        *out << endl;
                        cout << endl;
                    }
                    dfs(i, len + 1);
                }
            }
        }
    }
}

void countAndList(ofstream *o) {
    out = o;
    if (hasRing()) {
        printf("has ring\n");
        return;
    }
    totChain = count();
    *out << totChain << endl;
    cout << totChain << endl;
    FOR_ALPHA(i) {
        dfs(i, 0);
    }
    assert(totChain == 0);
    return;
}