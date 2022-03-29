#pragma once
#include "MaxHandler.h"

class MaxLetter : public MaxHandler {
private:
    int noRingHandler() {
        int inDegreee[26];
        if (headLetter) {
            headLetterInDegree(inDegreee);
        } else {
            getInDegree(inDegreee);
        }

        queue<int> q;
        int dp[26];
        int last[26];
        int selfLength[26];

        FOR_ALPHA(i) {
            if (edge[i][i]) {
                selfLength[i] = (int)word[i][i].back().length();
            } else {
                selfLength[i] = 0;
            }
        }
        memset(dp, 0, sizeof(dp));
        memset(last, 0, sizeof(last));
        FOR_ALPHA(i) {
            if (inDegreee[i] == 0) {
                q.push(i);
                dp[i] = selfLength[i];
                last[i] = -1;
            }
        }
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            FOR_ALPHA(i) {
                if (i != front && edge[front][i]) {
                    if (word[front][i].back().length() + selfLength[i] + dp[front] > dp[i]) {
                        dp[i] = (int)word[front][i].back().length() + selfLength[i] + dp[front];
                        last[i] = front;
                    }
                    inDegreee[i]--;
                    if (inDegreee[i] == 0) {
                        q.push(i);
                    }
                }
            }
        }
        int final, ret;
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
            cout << "no chain" << endl;
            return -1;
        }
        final = stk.top();
        stk.pop();
        while (!stk.empty()) {
            appendToResult(word[final][stk.top()].back());
            final = stk.top();
            stk.pop();
        }
        return ret;
    }

    int route[200], used[26][26];
    int maxRoute[26][26][200];
    int maxLength[26][26];
    int maxLetter[26][26];
    void dfs(int cur, int start, int length, int totLetter) {
        route[length] = cur;
        if (totLetter > maxLetter[start][cur]) {
            for (int i = 0; i <= length; i++) {
                maxRoute[start][cur][i] = route[i];
            }
            maxLength[start][cur] = length;
            maxLetter[start][cur] = totLetter;
        }
        for (auto& i : sccElement[sccBelong[start]]) {
            if (edge[cur][i] > used[cur][i]) {
                used[cur][i]++;
                dfs(i, start, length + 1, totLetter + (int)word[cur][i][(long long)edge[cur][i] - used[cur][i]].length());
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
        FOR_ALPHA(i) {
            FOR_ALPHA(j) {
                maxLetter[i][j] = -10000;
            }
        }
        FOR_SCC(i) {
            for (auto& j : sccInFromStart[i]) {
                if (sccInFromFinish[i].count(j)) {
                    memset(used, 0, sizeof(used));
                    maxLetter[j][j] = 0;
                    dfs(j, j, 0, 0);
                }
            }
        }
        queue<int> q;
        int innerLast[26], outerLast[26], maxInLetter[26], maxOutLetter[26];
        memset(innerLast, 0, sizeof(innerLast));
        memset(outerLast, 0, sizeof(outerLast));
        memset(maxInLetter, 0, sizeof(maxInLetter));
        memset(maxOutLetter, 0, sizeof(maxOutLetter));
        FOR_SCC(i) {
            if (sccInDegree[i] == 0) {
                q.push(i);
                for (auto& j : sccElement[i]) {
                    outerLast[j] = -1;
                }
            }
        }
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            for (auto& i : sccElement[front]) {
                for (auto& j : sccElement[front]) {
                    if (maxInLetter[j] + maxLetter[j][i] > maxOutLetter[i]) {
                        maxOutLetter[i] = maxInLetter[j] + maxLetter[j][i];
                        innerLast[i] = j;
                    }
                }
            }
            FOR_SCC(i) {
                if (sccEdge[front][i]) {
                    for (auto& j : sccElement[front]) {
                        for (auto& k : sccElement[i]) {
                            if (edge[j][k] && maxOutLetter[j] + word[j][k].back().length() > maxInLetter[k]) {
                                maxInLetter[k] = maxOutLetter[j] + (int)word[j][k].back().length();
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
        int final, ret;
        if (tailLetter) {
            final = tailLetter - 'a';
            ret = maxOutLetter[tailLetter - 'a'];
        } else {
            ret = 0;
            FOR_ALPHA(i) {
                if (maxOutLetter[i] > ret) {
                    ret = maxOutLetter[i];
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
            int* finish = maxRoute[entrance][exit] + maxLength[entrance][exit];
            for (int* i = maxRoute[entrance][exit] + 1; i <= finish; i++) {
                appendToResult(word[*(i - 1)][*i].back());
                word[*(i - 1)][*i].pop_back();
            }
            stk.pop();
            if (stk.empty()) {
                break;
            }
            appendToResult(word[exit][stk.top()].back());
            word[exit][stk.top()].pop_back();
        }
        return ret;
    }

public:
	MaxLetter(char head, char tail, bool allowRing, vector<string> word[26][26], char **result) :
	MaxHandler(head, tail, allowRing, word, result) {
        memset(route, 0, sizeof(route));
        memset(used, 0, sizeof(used));
        memset(maxRoute, 0, sizeof(maxRoute));
        memset(maxLength, 0, sizeof(maxLength));
        memset(maxLetter, 0, sizeof(maxLetter));
    }

	int handle() override {
        if (!allowRing) {
            if (hasRing()) {
                cout << "has ring" << endl;
                return -1;
            }
            return noRingHandler();
        }
        calcScc();
        return allowRingHandler();
	}
};

