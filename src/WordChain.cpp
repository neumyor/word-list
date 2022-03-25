#include "WordChain.h"
using namespace std;

vector<string> word[26][26];
int edge[26][26];
static void (*handler)(ofstream*);
static int read(int argc, char *argv[]) {
    FILE *file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            handler = countAndList;
        } else if (strcmp(argv[i], "-w") == 0) {
            handler = maxWord;
        } else {
            #ifdef __linux__
                file = fopen(argv[i], "r");
            #else
                fopen_s(&file, argv[i], "r");
            #endif
            if (file == NULL) {
                return -1;
            }
        }
    }
    char c;
    string tmp;
    char start = 0, last = 0;
    while ((c = fgetc(file)) && c != EOF) {
        if ('A' <= c && c <= 'Z') {
            c = c - 'A' + 'a';
        }
        if ('a' <= c && c <= 'z') {
            last = c;
            tmp += last;
            if (!start) {
                start = last;
            }
        } else if (tmp.length() > 0) {
            if (tmp.length() > 1) {
                word[start - 'a'][last - 'a'].push_back(tmp);
            }
            tmp.clear();
            start = 0;
        }
    }
    if (tmp.length() > 0) {
        if (tmp.length() > 1) {
            word[start - 'a'][last - 'a'].push_back(tmp);
        }
        tmp.clear();
        start = 0;
    }
    fclose(file);
    FOR_ALPHA(i) {
        FOR_ALPHA(j) {
            sort(word[i][j].begin(), word[i][j].begin() + edge[i][j], [](string &x, string &y) -> bool {
                return x.length() < y.length();
                });
            edge[i][j] = word[i][j].size();
        }
    }
    return 0;
}

void getInDegree(int *a) {
    FOR_ALPHA(i) {
        a[i] = 0;
        FOR_ALPHA(j) {
            if (i != j && edge[j][i]) {
                a[i]++;
            }
        }
    }
}

bool hasRing() {
    static int inDegree[26];
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

int main(int argc, char *argv[]) {
    if (read(argc, argv) < 0) {
        printf("File read failed\n");
        return 0;
    }
    if (handler == NULL) {
        printf("Unmatched params\n");
        return 0;
    }
    ofstream out("output.txt");
    (*handler)(&out);
    return 0;
}