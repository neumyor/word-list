#include "Core.h"
using namespace std;

static char headLetter = 0, tailLetter = 0;
static bool allowRing = false;

static void warning() {
    if (headLetter) {
        cout << "warning: head letter specification is ignored" << endl;
    }
    if (tailLetter) {
        cout << "warning: tail letter specification is ignored" << endl;
    }
    if (allowRing) {
        cout << "warning: cycle allowance is ignored" << endl;
    }
}

static inline bool isAlpha(char c) {
    if ('a' <= c && c <= 'z') {
        return true;
    }
    if ('A' <= c && c <= 'Z') {
        return true;
    }
    return false;
}

static inline char toLowercase(char c) {
    if ('a' <= c && c <= 'z') {
        return c;
    }
    return c - 'A' + 'a';
}

static void read(int argc, char *argv[]) {
    vector<char*> word;
    FILE *file = NULL;
    HandlerType type = HandlerType::UNKNOWN;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            if (type != HandlerType::UNKNOWN) {
                cout << "multiple type" << endl;
                return;
            }
            type = HandlerType::COUNT_AND_LIST;
        } else if (strcmp(argv[i], "-w") == 0) {
            if (type != HandlerType::UNKNOWN) {
                cout << "multiple type" << endl;
                return;
            }
            type = HandlerType::MAX_WORD;
        } else if (strcmp(argv[i], "-m") == 0) {
            if (type != HandlerType::UNKNOWN) {
                cout << "multiple type" << endl;
                return;
            }
            type = HandlerType::DISTINCT_INITIAL;
        } else if (strcmp(argv[i], "-c") == 0) {
            if (type != HandlerType::UNKNOWN) {
                cout << "multiple type" << endl;
                return;
            }
            type = HandlerType::MAX_LETTER;
        } else if (strcmp(argv[i], "-h") == 0) {
            i++;
            if (i >= argc || strlen(argv[i]) > 1 || !isAlpha(*argv[i])) {
                cout << "need letter after '-h'" << endl;
            } else if (headLetter) {
                cout << "multiple head" << endl;
            } else {
                headLetter = toLowercase(*argv[i]);
                continue;
            }
            if (file != NULL) {
                fclose(file);
            }
            return;
        } else if (strcmp(argv[i], "-t") == 0) {
            i++;
            if (i >= argc || strlen(argv[i]) > 1 || !isAlpha(*argv[i])) {
                cout << "need letter after '-t'" << endl;
            } else if (tailLetter) {
                cout << "multiple tail" << endl;
            } else {
                tailLetter = toLowercase(*argv[i]);
                continue;
            }
            if (file != NULL) {
                fclose(file);
            }
            return;
        } else if (strcmp(argv[i], "-r") == 0) {
            allowRing = true;
        } else {
            int l = (int)strlen(argv[i]);
            if (l < 4 || argv[i][l - 4] != '.' || argv[i][l - 3] != 't' || argv[i][l - 2] != 'x' || argv[i][l - 1] != 't') {
                cout << argv[i] << " is neither a text file nor an argument" << endl;
                return;
            }
            if (file != NULL) {
                cout << "multiple files found" << endl;
                fclose(file);
                return;
            }
            #ifdef __linux__
                file = fopen(argv[i], "r");
            #else
                fopen_s(&file, argv[i], "r");
            #endif
            if (file == NULL) {
                cout << "can't find file" << endl;
                return;
            }
        }
    }
    if (file == NULL) {
        cout << "no input file" << endl;
        return;
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
                char *c = (char*)malloc(tmp.length() + 1);
                for (int i = 0; i < (int)tmp.length(); i++) {
                    c[i] = tmp[i];
                }
                c[tmp.length()] = 0;
                word.push_back(c);
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
            char *c = (char*)malloc(tmp.length() + 1);
            if (c) {
                for (int i = 0; i < (int)tmp.length(); i++) {
                    c[i] = tmp[i];
                }
                c[tmp.length()] = 0;
                word.push_back(c);
            }
        }
        tmp.clear();
        start = 0;
    #pragma   warning(pop)  
    }

    fclose(file);
    static char *result[MAX_RESULT_LINE + 5];
    int ret;
    ofstream fout("solution.txt");
    switch (type) {
    case HandlerType::COUNT_AND_LIST:
        warning();
        ret = Core::gen_chains_all(&word[0], (int)word.size(), result);
        if (ret > MAX_RESULT_LINE) {
            cout << "too many chains" << endl;
            fout << "too many chains" << endl;
        } else if (ret >= 0) {
            cout << ret << endl;
            fout << ret << endl;
        }
        break;
    case HandlerType::DISTINCT_INITIAL:
        warning();
        ret = Core::gen_chain_word_unique(&word[0], (int)word.size(), result);
        break;
    case HandlerType::MAX_WORD:
        ret = Core::gen_chain_word(&word[0], (int)word.size(), result, headLetter, tailLetter, allowRing);
        break;
    case HandlerType::MAX_LETTER:
        ret = Core::gen_chain_char(&word[0], (int)word.size(), result, headLetter, tailLetter, allowRing);
        break;
    default:
        cout << "unmatch type" << endl;
        return;
    }

    if (ret == -1) {
        cout << "has cycle" << endl;
        return;
    }
    for (int i = 0; result[i] != nullptr; i++) {
        cout << result[i] << endl;
        fout << result[i] << endl;
        free(result[i]);
        result[i] = nullptr;
    }
    
    for (auto &i : word) {
        free(i);
    }
    if (ret > MAX_RESULT_LINE) {
        cout << "..." << endl;
    }
    cout << "return value: " << ret << endl;
}

#ifdef Release

int main(int argc, char *argv[]) {
    read(argc, argv);

    headLetter = 0;
    tailLetter = 0;
    allowRing = false;
    return 0;
}

#endif // Release




#ifdef _WIN32
extern "C" {
    __declspec(dllexport) char* __stdcall call_by_cmd(int len, char* cmd);
    __declspec(dllexport) int __stdcall gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
    __declspec(dllexport) int __stdcall gen_chains_all(char* words[], int len, char* result[]);
    __declspec(dllexport) int __stdcall gen_chain_word_unique(char* words[], int len, char* result[]);
    __declspec(dllexport) int __stdcall gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
}

char* temp_ret_ptr = NULL;
__declspec(dllexport) char* __stdcall call_by_cmd(int len, char* cmd) {
    if (temp_ret_ptr) {
        delete[] temp_ret_ptr;
        temp_ret_ptr = NULL;
    }

    istringstream input_cmd(cmd);
    vector<string> temp_vec(0);
    string out;

    while (input_cmd >> out) {
        temp_vec.push_back(out);
    }

    const int argc = (int)temp_vec.size();
    char** argv = new char* [argc];
    for (int i = 0; i < argc; i++) {
        argv[i] = new char[temp_vec[i].size() + 1];
        strcpy_s(argv[i], temp_vec[i].size() + 1, temp_vec[i].c_str());
    }

    stringstream ss;
    streambuf* buffer = cout.rdbuf();
    cout.rdbuf(ss.rdbuf());

    read(argc, argv);

    string ret(ss.str());
    cout.rdbuf(buffer);

    ret += '\0';
    temp_ret_ptr = new char[ret.size() + 1];
    strcpy_s(temp_ret_ptr, ret.size() + 1, ret.c_str());

    headLetter = 0;
    tailLetter = 0;
    allowRing = false;

    return temp_ret_ptr;
}

#ifdef Release
__declspec(dllexport) int __stdcall gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop) {
    return Core::gen_chain_word(words, len, result, head, tail, enable_loop);
}

__declspec(dllexport) int __stdcall gen_chains_all(char* words[], int len, char* result[]) {
    return Core::gen_chains_all(words, len, result);
}

__declspec(dllexport) int __stdcall gen_chain_word_unique(char* words[], int len, char* result[]) {
    return Core::gen_chain_word_unique(words, len, result);
}

__declspec(dllexport) int __stdcall gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop) {
    return Core::gen_chain_char(words, len, result, head, tail, enable_loop);
}
#endif // Release
#endif