#pragma once
#include "Handler.h"

class CountAndList : public Handler {
private:
    int dp[26];
    int inDegree[26];
    int totChain;
    string* previous[100];

    void dfs(int cur, int len) {
        FOR_ALPHA(i) {
            if (i != cur && edge[cur][i]) {
                for (int j = ((int)word[cur][i].size()) - 1; j >= 0; j--) {
                    previous[len + 1] = word[cur][i][j];
                    if (len >= 1) {
                        int size = 0;
                        for (int k = 1; k <= len + 1; k++) {
                            size += (int)previous[k]->size() + 1;
                        }
                        result[line] = (char*)malloc(size);
                        int length = 0;
                        for (int k = 1; k <= len; k++) {
                            for (int u = 0; u < (int)previous[k]->size(); u++) {
                                result[line][length++] = (*previous[k])[u];
                            }
                            result[line][length++] = ' ';
                        }
                        for (int k = 0; k < (int)previous[len + 1]->size(); k++) {
                            result[line][length++] = (*previous[len + 1])[k];
                        }
                        result[line][length++] = '\0';
                        line++;
                        totChain++;
                        if (totChain > MAX_RESULT_LINE) {
                            return;
                        }
                    }
                    dfs(i, len + 1);
                    if (totChain > MAX_RESULT_LINE) {
                        return;
                    }
                }
            }
        }
        if (edge[cur][cur]) {
            len++;
            previous[len] = word[cur][cur][0];
            if (len >= 2) {
                int size = 0;
                for (int k = 1; k <= len; k++) {
                    size += (int)previous[k]->size() + 1;
                }
                result[line] = (char*)malloc(size);
                int length = 0;
                for (int k = 1; k < len; k++) {
                    for (int u = 0; u < (int)previous[k]->size(); u++) {
                        result[line][length++] = (*previous[k])[u];
                    }
                    result[line][length++] = ' ';
                }
                for (int k = 0; k < (int)previous[len]->size(); k++) {
                    result[line][length++] = (*previous[len])[k];
                }
                result[line][length++] = '\0';
                line++;
                totChain++;
                if (totChain > MAX_RESULT_LINE) {
                    return;
                }
            }
            FOR_ALPHA(i) {
                if (i != cur && edge[cur][i]) {
                    for (int j = ((int)word[cur][i].size()) - 1; j >= 0; j--) {
                        previous[len + 1] = word[cur][i][j];
                        if (len >= 1) {
                            int size = 0;
                            for (int k = 1; k <= len + 1; k++) {
                                size += (int)previous[k]->size() + 1;
                            }
                            result[line] = (char*)malloc(size);
                            int length = 0;
                            for (int k = 1; k <= len; k++) {
                                for (int u = 0; u < (int)previous[k]->size(); u++) {
                                    result[line][length++] = (*previous[k])[u];
                                }
                                result[line][length++] = ' ';
                            }
                            for (int k = 0; k < (int)previous[len + 1]->size(); k++) {
                                result[line][length++] = (*previous[len + 1])[k];
                            }
                            result[line][length++] = '\0';
                            line++;
                            totChain++;
                            if (totChain > MAX_RESULT_LINE) {
                                return;
                            }
                        }
                        dfs(i, len + 1);
                        if (totChain > MAX_RESULT_LINE) {
                            return;
                        }
                    }
                }
            }
        }
    }

public:
	CountAndList(StringSet word[26][26], char **result) : Handler(word, result) {
        totChain = 0;
        memset(dp, 0, sizeof(dp));
        memset(previous, 0, sizeof(previous));
    }

	int handle() override {
        if (hasRing()) {
            printf("has ring\n");
            return -1;
        }
        result[line] = (char*)malloc(10);

        FOR_ALPHA(i) {
            dfs(i, 0);
            if (totChain > MAX_RESULT_LINE) {
                break;
            }
        }
        return totChain;
	}
};

