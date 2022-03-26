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

static int route[200], used[26][26];
static int maxRoute[26][26][200];
static int maxLength[26][26];
static int maxLetter[26][26];
static void dfs(int cur, int start, int length, int totLetter) {
    route[length] = cur;
    if (totLetter > maxLetter[start][cur]) {
        for (int i = 0; i <= length; i++) {
            maxRoute[start][cur][i] = route[i];
        }
        maxLength[start][cur] = length;
        maxLetter[start][cur] = totLetter;
    }
    for (auto & i : sccElement[sccBelong[start]]) {
        if (edge[cur][i] > used[cur][i]) {
            used[cur][i]++;
            dfs(i, start, length + 1, totLetter + word[cur][i][edge[cur][i] - used[cur][i]].length());
            used[cur][i]--;
        }
    }
}

static void allowRingHandler(ofstream *out) {
    if (headLetter) {
        bfsFromStart(headLetter - 'a');
    } else {
        FOR_ALPHA(i) {
            if (sccInDegree[sccBelong[i]] == 0) {
                bfsFromStart(i);
            }
        }
    }
    if (tailLetter) {
        bfsFromFinish(tailLetter - 'a');
    } else {
        FOR_ALPHA(i) {
            if (sccOutDegree[sccBelong[i]] == 0) {
                bfsFromFinish(i);
            }
        }
    }
    FOR_ALPHA(i) {
        FOR_ALPHA(j) {
            maxLetter[i][j] = -10000;
        }
    }
    FOR_SCC(i) {
        for (auto &j : sccInFromStart[i]) {
            if (sccInFromFinish[i].count(j)) {
                memset(used, 0, sizeof(used));
                maxLetter[j][j] = 0;
                dfs(j, j, 0, 0);
            }
        }
    }
    queue<int> q;
    static int innerLast[26], outerLast[26], maxInLetter[26], maxOutLetter[26];
    FOR_SCC(i) {
        if (sccInDegree[i] == 0) {
            q.push(i);
            for (auto &j : sccElement[i]) {
                outerLast[j] = -1;
            }
        }
    }
    while (!q.empty()) {
        int front = q.front();
        q.pop();
        for (auto &i : sccElement[front]) {
            for (auto &j : sccElement[front]) {
                if (maxInLetter[j] + maxLetter[j][i] > maxOutLetter[i]) {
                    maxOutLetter[i] = maxInLetter[j] + maxLetter[j][i];
                    innerLast[i] = j;
                }
            }
        }
        FOR_SCC(i) {
            if (sccEdge[front][i]) {
                for (auto &j : sccElement[front]) {
                    for (auto &k : sccElement[i]) {
                        if (edge[j][k] && maxOutLetter[j] + word[j][k].back().length() > maxInLetter[k]) {
                            maxInLetter[k] = maxOutLetter[j] + word[j][k].back().length();
                            outerLast[k] = j;
                        }
                    }
                }
                sccInDegree[i]--;
                if (sccInDegree[i] == 0) {
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
            if (maxOutLetter[i] > mx) {
                mx = maxOutLetter[i];
                final = i;
            }
        }
    }
    stack<int> stk;
    while (final >= 0) {
        stk.push(final);
        stk.push(innerLast[final]);
        final = outerLast[final];
    }
    while (true) {
        int entrance = stk.top();
        stk.pop();
        int exit = stk.top();
        int *finish = maxRoute[entrance][exit] + maxLength[entrance][exit];
        for (int *i = maxRoute[entrance][exit] + 1; i <= finish; i++) {
            *out << word[*(i - 1)][*i].back() << endl;
            cout << word[*(i - 1)][*i].back() << endl;
            word[*(i - 1)][*i].pop_back();
        }
        stk.pop();
        if (stk.empty()) {
            break;
        }
        *out << word[exit][stk.top()].back() << endl;
        cout << word[exit][stk.top()].back() << endl;
        word[exit][stk.top()].pop_back();
    }
}

void maxWord(ofstream *out) {
    if (!allowRing) {
        if (hasRing()) {
            cout << "has ring" << endl;
            return;
        }
        noRingHandler(out);
        return;
    }
    calcScc();
    allowRingHandler(out);
}