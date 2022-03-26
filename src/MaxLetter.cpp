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

int route[200], used[26][26];
int maxRoute[26][26][200];
int maxLength[26][26];
static void dfs(int cur, int start, int length) {
    route[length] = cur;
    if (length > maxLength[start][cur]) {
        for (int i = 0; i <= length; i++) {
            maxRoute[start][cur][i] = route[i];
        }
        maxLength[start][cur] = length;
    }
    for (auto & i : sccElement[sccBelong[start]]) {
        if (edge[cur][i] > used[cur][i]) {
            used[cur][i]++;
            dfs(i, start, length + 1);
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
    FOR_SCC(i) {
        for (auto &j : sccInFromStart[i]) {
            if (sccInFromFinish[i].count(j)) {
                memset(used, 0, sizeof(used));
                dfs(j, j, 0);
            }
        }
    }
    queue<int> q;
    static int innerLast[26], outerLast[26], maxInLength[26], maxOutLength[26];
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
                if (maxInLength[j] + maxLength[j][i] > maxOutLength[i]) {
                    maxOutLength[i] = maxInLength[j] + maxLength[j][i];
                    innerLast[i] = j;
                }
            }
        }
        FOR_SCC(i) {
            if (sccEdge[front][i]) {
                for (auto &j : sccElement[front]) {
                    for (auto &k : sccElement[i]) {
                        if (edge[j][k] && maxOutLength[j] + 1 > maxInLength[k]) {
                            maxInLength[k] = maxOutLength[j] + 1;
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
            if (maxOutLength[i] > mx) {
                mx = maxOutLength[i];
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

void maximizeLetter(ofstream *out) {
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