#include "Core.h"
using namespace std;

static void read(int argc, char *argv[]) {
    vector<char*> word;
    char headLetter = 0, tailLetter = 0;
    bool allowRing = false;
    FILE *file = NULL;
    HandlerType type = HandlerType::UNKNOWN;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            type = HandlerType::COUNT_AND_LIST;
        } else if (strcmp(argv[i], "-w") == 0) {
            type = HandlerType::MAX_WORD;
        } else if (strcmp(argv[i], "-m") == 0) {
            type = HandlerType::DISTINCT_INITIAL;
        } else if (strcmp(argv[i], "-c") == 0) {
            type = HandlerType::MAX_LETTER;
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
        }
    }
    if (file == NULL) {
        cout << "can't find file" << endl;
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
                for (int i = 0; i < tmp.length(); i++) {
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
                for (int i = 0; i < tmp.length(); i++) {
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
    //static char* result = new char* [MAX_RESULT_LINE];
    static char *result[MAX_RESULT_LINE];
    switch (type) {
    case HandlerType::COUNT_AND_LIST:
        Core::gen_chains_all(&word[0], (int)word.size(), result);
        break;
    case HandlerType::DISTINCT_INITIAL:
        Core::gen_chain_word_unique(&word[0], (int)word.size(), result);
        break;
    case HandlerType::MAX_WORD:
        Core::gen_chain_word(&word[0], (int)word.size(), result, headLetter, tailLetter, allowRing);
        break;
    case HandlerType::MAX_LETTER:
        Core::gen_chain_char(&word[0], (int)word.size(), result, headLetter, tailLetter, allowRing);
        break;
    default:
        cout << "unmatch type" << endl;
        return;
    }
    
    for (int i = 0; result[i] != nullptr; i++) {
        cout << result[i] << endl;
        free(result[i]);
        result[i] = nullptr;
        
    }
    
    for (auto &i : word) {
        free(i);
    }
    

}

int main(int argc, char *argv[]) {
    read(argc, argv);
    return 0;
}

#ifdef _WIN32
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

    stringstream ss;
    streambuf* buffer = cout.rdbuf();
    cout.rdbuf(ss.rdbuf());

    read(argc, argv);

    string ret(ss.str());
    cout.rdbuf(buffer);


    if (temp_ret_ptr) {
        cout << "C++ first" << endl;
        delete[] temp_ret_ptr;
        temp_ret_ptr = NULL;
        cout << "C++ next" << endl;
    }
    ret += '\0';
    cout << ret << endl;
    temp_ret_ptr = new char[ret.size() + 1];
    strcpy_s(temp_ret_ptr, ret.size() + 1, ret.c_str());

    return temp_ret_ptr;
}
#endif