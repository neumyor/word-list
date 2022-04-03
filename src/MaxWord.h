#pragma once
#include "MaxHandler.h"

class MaxWord : public MaxHandler {
private:
    int route[200], used[26][26];
    int maxRoute[26][26][200];
    int maxLength[26][26];
    int noRingHandler() {
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
        int final = -1, ret;
        if (tailLetter) {
            final = tailLetter - 'a';
            ret = dp[tailLetter - 'a'];
        } else {
            ret = 0;
            FOR_ALPHA(i) {
                if (dp[i] > ret) {
                    ret = dp[i];
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
            return 0;
        }
        final = stk.top();
        stk.pop();
        while (!stk.empty()) {
            appendToResult(word[final][stk.top()][0]);
            final = stk.top();
            stk.pop();
        }
        return ret;
    }

    void dfs(int cur, int start, int length) {
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

    int allowRingHandler() {
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
        int innerLast[26], outerLast[26], maxInLength[26], maxOutLength[26];
        memset(innerLast, 0, sizeof(innerLast));
        memset(maxInLength, 0, sizeof(maxInLength));
        memset(maxOutLength, 0, sizeof(maxOutLength));
        FOR_SCC(i) {
            if (sccInDegree[i] == 0) {
                q.push(i);
            }
        }
        FOR_ALPHA(i) {
            outerLast[i] = -1;
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
        int final = -1, ret;
        if (tailLetter) {
            ret = maxOutLength[tailLetter - 'a'];
            final = tailLetter - 'a';
        } else {
            ret = 0;
            FOR_ALPHA(i) {
                if (maxOutLength[i] > ret) {
                    ret = maxOutLength[i];
                    final = i;
                }
            }
        }
        stack<int> stk;
        while (final >= 0) {
            stk.push(final);
            stk.push(innerLast[final]);
            final = outerLast[innerLast[final]];
        }
        if (stk.size() < 2) {
            return 0;
        }
        while (true) {
            int entrance = stk.top();
            stk.pop();
            int exit = stk.top();
            int *finish = maxRoute[entrance][exit] + maxLength[entrance][exit];
            for (int *i = maxRoute[entrance][exit] + 1; i <= finish; i++) {
                appendToResult(word[*(i - 1)][*i].back());
                delete word[*(i - 1)][*i].back();
                word[*(i - 1)][*i].pop_back();
            }
            stk.pop();
            if (stk.empty()) {
                break;
            }
            appendToResult(word[exit][stk.top()].back());
            delete word[exit][stk.top()].back();
            word[exit][stk.top()].pop_back();
        }
        return ret;
    }
public:
    MaxWord(char head, char tail, bool allowRing, StringSet word[26][26], char **result) :
        MaxHandler(head, tail, allowRing, word, result) {
        memset(route, 0, sizeof(route));
        memset(used, 0, sizeof(used));
        memset(maxRoute, 0, sizeof(maxRoute));
        memset(maxLength, 0, sizeof(maxLength));
    }

	int handle() override {
        if (!allowRing) {
            if (hasRing()) {
                return -1;
            }
            return noRingHandler();
        }
        calcScc();
        return allowRingHandler();
	}
};

