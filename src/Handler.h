#pragma once
#include "WordChain.h"

class Handler {
protected:
	char headLetter;
	char tailLetter;
	bool allowRing;
	vector<string> word[26][26];
	int edge[26][26];
    int line;
    char **result;
    void my_strcpy(char* dst, const char* src, int size) {
        while (size--) {
            *dst = *src;
            dst++;
            src++;
        }
    }
    void appendToResult(string &s) {
        
        result[line] = (char*)malloc(s.size() + 1);
        my_strcpy(result[line], s.c_str(), (int)s.size());
        result[line][s.size()] = '\0';
        line++;
        
    }

private:
	void setEdge(vector<string> word[26][26]) {
		FOR_ALPHA(i) {
			FOR_ALPHA(j) {
				this->word[i][j] = move(word[i][j]);
				edge[i][j] = (int)this->word[i][j].size();
			}
		}
	}

protected:
    void getInDegree(int* a) {
        FOR_ALPHA(i) {
            a[i] = 0;
            FOR_ALPHA(j) {
                if (i != j && edge[j][i]) {
                    a[i]++;
                }
            }
        }
    }

    void headLetterInDegree(int* a) {
        FOR_ALPHA(i) {
            a[i] = 0;
        }
        queue<int> q;
        q.push(headLetter - 'a');
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            FOR_ALPHA(i) {
                if (i != front && edge[front][i]) {
                    if (a[i] == 0) {
                        q.push(i);
                    }
                    a[i]++;
                }
            }
        }
        FOR_ALPHA(i) {
            if (a[i] == 0) {
                a[i] = 0x3f3f3f;
            }
        }
        a[headLetter - 'a'] = 0;
    }
	
    bool hasRing() {
        int inDegree[26];
        FOR_ALPHA(i) {
            if (edge[i][i] > 1) {
                return true;
            }
        }
        getInDegree(inDegree);
        queue<int> q;
        int cnt = 0;
        FOR_ALPHA(i) {
            if (inDegree[i] == 0) {
                q.push(i);
                cnt++;
            }
        }
        while (!q.empty()) {
            int front = q.front();
            q.pop();
            FOR_ALPHA(i) {
                if (i != front && edge[front][i]) {
                    inDegree[i]--;
                    if (inDegree[i] == 0) {
                        q.push(i);
                        cnt++;
                    }
                }
            }
        }
        return cnt != 26;
    }
public:
	Handler(vector<string> word[26][26], char **result) : result(result), line(0) {
		setEdge(word);
	}

	Handler(char head, char tail, bool allowRing, vector<string> word[26][26], char **result) : headLetter(head), tailLetter(tail),
		allowRing(allowRing), result(result), line(0) {
		setEdge(word);
	}

	virtual int handle() = 0;
};
