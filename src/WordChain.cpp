#include "WordChain.h"
using namespace std;

vector<string> word[26][26];
int edge[26][26];
char headLetter, tailLetter;
bool allowRing;
static void (*handler)(ofstream*);
static int read(int argc, char *argv[]) {
    FILE *file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            handler = countAndList;
        } else if (strcmp(argv[i], "-w") == 0) {
            handler = maxWord;
        } else if (strcmp(argv[i], "-m") == 0) {
            handler = distinctInitial;
        } else if (strcmp(argv[i], "-c") == 0) {
            handler = maximizeLetter;
        } else if (strcmp(argv[i], "-h") == 0) {
            i++;
            headLetter = *argv[i];
        } else if (strcmp(argv[i], "-t") == 0) {
            i++;
            tailLetter = *argv[i];
        } else if (strcmp(argv[i], "-r") == 0) {
            allowRing = true;
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

    if (file == NULL) {
        return -1;
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
    #pragma warning(push)
    #pragma warning(disable:6385)
    #pragma warning(disable:6386)
        if (tmp.length() > 1) {
            word[start - 'a'][last - 'a'].push_back(tmp);
        }
        tmp.clear();
        start = 0;
    #pragma   warning(pop)  
    }
    fclose(file);
    FOR_ALPHA(i) {
        FOR_ALPHA(j) {
            sort(word[i][j].begin(), word[i][j].begin() + edge[i][j], [](string &x, string &y) -> bool {
                return x.length() < y.length();
                });
            edge[i][j] = (int) word[i][j].size();
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

void headLetterInDegree(int *a) {
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

int sccBelong[26];
int sccNumber = 0;
static int dfn[26], low[26];
vector<int> sccElement[26];
static void dfsScc(int x) {
    static int depth = 0;
    static stack<int> stk;
    static bool inStack[26];
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

bool sccEdge[26][26];
int sccInDegree[26];
int sccOutDegree[26];
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

set<int> sccInFromStart[26];
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

set<int> sccInFromFinish[26];
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

extern "C" {
    __declspec(dllexport) char* __stdcall call_by_cmd(int len, char* cmd);
}

char* temp_ret_ptr = NULL;
char* call_by_cmd(int len, char* cmd) {
    temp_ret_ptr = NULL;

    istringstream input_cmd(cmd);
    vector<string> temp_vec(0);
    string out;

    while (input_cmd >> out) {
        temp_vec.push_back(out);
    }

    const int argc = (int)temp_vec.size();
    char** argv = new char* [argc];
    for (int i = 0; i < argc; i++) {
        cout << temp_vec[i] << endl;
        argv[i] = new char[temp_vec[i].size() + 1];
        strcpy_s(argv[i], temp_vec[i].size() + 1, temp_vec[i].c_str());
    }

    if (read(argc, argv) < 0) {
        printf("File read failed\n");
        return 0;
    }

    if (handler == NULL) {
        printf("Unmatched params\n");
        return 0;
    }

    stringstream ss;
    streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
    cout.rdbuf(ss.rdbuf());

    ofstream out_stream("output.txt");
    (*handler)(&out_stream);

    string ret(ss.str());
    cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区

    delete[] temp_ret_ptr;
    temp_ret_ptr = new char[strlen(ret.c_str()) + 1];
    strcpy_s(temp_ret_ptr, strlen(ret.c_str()) + 1, ret.c_str());

    return temp_ret_ptr;
}