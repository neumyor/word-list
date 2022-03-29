#pragma once
#include "Handler.h"
class DistinctInitial : public Handler {
private:
    int ans[26];
    int last[26];
public:
	DistinctInitial(vector<string> word[26][26], char **result) : Handler(word, result) {
        memset(ans, 0, sizeof(ans));
        memset(last, 0, sizeof(last));
    }

    int handle() override {
        
        if (hasRing()) {
            printf("has ring!\n");
            return -1;
        }
        queue<int> q;
        int inDegree[26];
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
        int ret = 0, final;
        FOR_ALPHA(i) {
            if (ans[i] > ret) {
                ret = ans[i];
                final = i;
            }
        }
        
        if (ret == 0) {
            FOR_ALPHA(i) {
                if (edge[i][i]) {
                    appendToResult(word[i][i][0]);
                    return 1;
                }
            }
            return -1;
        }
        
        while (final >= 0) {
            stk.push(final);
            final = last[final];
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
};

