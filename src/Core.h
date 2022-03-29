#include "WordChain.h"
#include "MaxLetter.h"
#include "MaxWord.h"
#include "CountAndList.h"
#include "DistinctInitial.h"

struct Core {
    #pragma warning(push)
    #pragma warning(disable:6262)
    static int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop) {
        vector<string> word[26][26];
        for (int i = 0; i < len; i++) {
            string tmp = words[i];
            word[tmp[0] - 'a'][tmp.back() - 'a'].push_back(tmp);
        }
        return (new MaxWord(head, tail, enable_loop, word, result))->handle();
    }

    static int gen_chains_all(char* words[], int len, char* result[]) {
        vector<string> word[26][26];
        for (int i = 0; i < len; i++) {
            string tmp = words[i];
            word[tmp[0] - 'a'][tmp.back() - 'a'].push_back(tmp);
        }
        return (new CountAndList(word, result))->handle();
    }

    static int gen_chain_word_unique(char* words[], int len, char* result[]) {
        vector<string> word[26][26];
        for (int i = 0; i < len; i++) {
            string tmp = words[i];
            word[tmp[0] - 'a'][tmp.back() - 'a'].push_back(tmp);
        }
        return (new DistinctInitial(word, result))->handle();
    }

    static int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop) {
        vector<string> word[26][26];
        for (int i = 0; i < len; i++) {
            string tmp = words[i];
            word[tmp[0] - 'a'][tmp.back() - 'a'].push_back(tmp);
        }
        return (new MaxLetter(head, tail, enable_loop, word, result))->handle();
    }
    #pragma warning(pop)
};