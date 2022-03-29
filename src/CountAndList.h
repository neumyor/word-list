#pragma once
#include "Handler.h"

class CountAndList : public Handler {
private:
    int dp[26];
    int inDegree[26];
    int totChain;
    string* previous[100];
    int count() {
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
        int res = 0;
        FOR_ALPHA(i) {
            res += dp[i] - 1;
            FOR_ALPHA(j) {
                res -= edge[i][j];
            }
        }
        return res;
    }

    void dfs(int cur, int len) {
        FOR_ALPHA(i) {
            if (i != cur && edge[cur][i]) {
                for (int j = ((int)word[cur][i].size()) - 1; j >= 0; j--) {
                    previous[len + 1] = &word[cur][i][j];
                    if (len >= 1) {
                        int size = 0;
                        for (int k = 1; k <= len + 1; k++) {
                            size += (int)previous[k]->size() + 1;
                        }
                        result[line] = (char*)malloc(size);
                        int length = 0;
                        for (int k = 1; k <= len; k++) {
                            for (int u = 0; u < previous[k]->size(); u++) {
                                result[line][length++] = (*previous[k])[u];
                            }
                            result[line][length++] = ' ';
                        }
                        for (int k = 0; k < previous[len + 1]->size(); k++) {
                            result[line][length++] = (*previous[len + 1])[k];
                        }
                        result[line][length++] = '\0';
                        line++;
                        totChain--;
                    }
                    dfs(i, len + 1);
                }
            }
        }
        if (edge[cur][cur]) {
            len++;
            previous[len] = &word[cur][cur][0];
            if (len >= 2) {
                int size = 0;
                for (int k = 1; k <= len; k++) {
                    size += (int)previous[k]->size() + 1;
                }
                result[line] = (char*)malloc(size);
                int length = 0;
                for (int k = 1; k < len; k++) {
                    for (int u = 0; u < previous[k]->size(); u++) {
                        result[line][length++] = (*previous[k])[u];
                    }
                    result[line][length++] = ' ';
                }
                for (int k = 0; k < previous[len]->size(); k++) {
                    result[line][length++] = (*previous[len])[k];
                }
                result[line][length++] = '\0';
                line++;
                totChain--;
            }
            FOR_ALPHA(i) {
                if (i != cur && edge[cur][i]) {
                    for (int j = ((int)word[cur][i].size()) - 1; j >= 0; j--) {
                        previous[len + 1] = &word[cur][i][j];
                        if (len >= 1) {
                            int size = 0;
                            for (int k = 1; k <= len + 1; k++) {
                                size += (int)previous[k]->size() + 1;
                            }
                            result[line] = (char*)malloc(size);
                            int length = 0;
                            for (int k = 1; k <= len; k++) {
                                for (int u = 0; u < previous[k]->size(); u++) {
                                    result[line][length++] = (*previous[k])[u];
                                }
                                result[line][length++] = ' ';
                            }
                            for (int k = 0; k < previous[len + 1]->size(); k++) {
                                result[line][length++] = (*previous[len + 1])[k];
                            }
                            result[line][length++] = '\0';
                            line++;
                            totChain--;
                        }
                        dfs(i, len + 1);
                    }
                }
            }
        }
    }

public:
	CountAndList(vector<string> word[26][26], char **result) : Handler(word, result) {
        totChain = 0;
        memset(dp, 0, sizeof(dp));
        memset(previous, 0, sizeof(previous));
    }

	int handle() override {
        if (hasRing()) {
            printf("has ring\n");
            return -1;
        }
        int ret;
        ret = totChain = count();
        result[line] = (char*)malloc(10);

        string tmp = to_string(totChain);
        tmp += '\0';

        if (result[line]) {
            strcpy_s(result[line], tmp.size() + 1, tmp.c_str());
        }
        
        line++;
        
        FOR_ALPHA(i) {
            dfs(i, 0);
        }
        assert(totChain == 0);
        return ret;
	}
};

