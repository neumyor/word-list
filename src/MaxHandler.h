#pragma once
#include "Handler.h"

class MaxHandler : public Handler {
private:
    int dfn[26], low[26];
    int depth;
    stack<int> stk;
    bool inStack[26];

protected:
    int sccBelong[26];
    int sccNumber = 0;
    vector<int> sccElement[26];
    bool sccEdge[26][26];
    int sccInDegree[26];
    int sccOutDegree[26];
    set<int> sccInFromFinish[26];
    set<int> sccInFromStart[26];

    void dfsScc(int x) {
        dfn[x] = low[x] = ++depth;
        stk.push(x);
        inStack[x] = true;
        FOR_ALPHA(i) {
            if (i == x || !edge[x][i]) {
                continue;
            }
            if (!dfn[i]) {
                dfsScc(i);
                low[x] = min(low[x], low[i]);
            } else if (inStack[i]) {
                low[x] = min(low[x], dfn[i]);
            }
        }
        if (dfn[x] == low[x]) {
            int tmp;
            do {
                tmp = stk.top();
                stk.pop();
                inStack[tmp] = false;
                sccElement[sccNumber].push_back(tmp);
                sccBelong[tmp] = sccNumber;
            } while (tmp != x);
            sccNumber++;
        }
    }

    void calcScc() {
        FOR_ALPHA(i) {
            if (dfn[i] == 0) {
                dfsScc(i);
            }
        }
        FOR_ALPHA(i) {
            FOR_ALPHA(j) {
                if (sccBelong[i] != sccBelong[j] && edge[i][j]) {
                    sccEdge[sccBelong[i]][sccBelong[j]] = true;
                }
            }
        }
        if (headLetter) {
            queue<int> q;
            q.push(sccBelong[headLetter - 'a']);
            while (!q.empty()) {
                int front = q.front();
                q.pop();
                FOR_SCC(i) {
                    if (sccEdge[front][i]) {
                        if (sccInDegree[i] == 0) {
                            q.push(i);
                        }
                        sccInDegree[i]++;
                        sccOutDegree[front]++;
                    }
                }
            }
            FOR_SCC(i) {
                if (sccInDegree[i] == 0) {
                    sccInDegree[i] = 0x3f3f3f;
                }
            }
            sccInDegree[sccBelong[headLetter - 'a']] = 0;
            return;
        }
        FOR_SCC(i) {
            FOR_SCC(j) {
                if (sccEdge[i][j]) {
                    sccInDegree[j]++;
                    sccOutDegree[i]++;
                }
            }
        }
    }

    void bfsFromStart(int start) {
        bool vis[26];
        memset(vis, false, sizeof(vis));
        queue<int> q;
        q.push(start);
        vis[start] = true;
        sccInFromStart[sccBelong[start]].insert(start);
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            FOR_ALPHA(i) {
                if (edge[front][i]) {
                    if (!vis[i]) {
                        q.push(i);
                        vis[i] = true;
                    }
                    if (sccBelong[front] != sccBelong[i]) {
                        sccInFromStart[sccBelong[i]].insert(i);
                    }
                }
            }
        }
    }

    void bfsFromFinish(int finish) {
        bool vis[26];
        memset(vis, false, sizeof(vis));
        queue<int> q;
        q.push(finish);
        vis[finish] = true;
        sccInFromFinish[sccBelong[finish]].insert(finish);
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            FOR_ALPHA(i) {
                if (edge[i][front]) {
                    if (!vis[i]) {
                        q.push(i);
                        vis[i] = true;
                    }
                    if (sccBelong[front] != sccBelong[i] || !sccInDegree[sccBelong[front]]) {
                        sccInFromFinish[sccBelong[front]].insert(front);
                    }
                }
            }
        }
    }
public:
	MaxHandler(char head, char tail, bool allowRing, StringSet word[26][26], char **result) : 
	Handler(head, tail, allowRing, word, result), depth(0) {
        memset(dfn, 0, sizeof(dfn));
        memset(low, 0, sizeof(low));
        memset(inStack, 0, sizeof(inStack));
        memset(sccEdge, 0, sizeof(sccEdge));
        memset(sccInDegree, 0, sizeof(sccInDegree));
        memset(sccOutDegree, 0, sizeof(sccOutDegree));
        memset(sccBelong, 0, sizeof(sccBelong));
    }
};

