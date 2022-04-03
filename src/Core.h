#pragma once
#include "WordChain.h"
#include "MaxLetter.h"
#include "MaxWord.h"
#include "CountAndList.h"
#include "DistinctInitial.h"

struct Core {
    static inline void construct(char *words[], int len, StringSet word[26][26]) {
        for (int i = 0; i < len; i++) {
            auto tmp = new string(words[i]);
            word[(*tmp)[0] - 'a'][tmp->back() - 'a'].insert(tmp);
        }
    }

    #pragma warning(push)
    #pragma warning(disable:6262)
    static int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop) {
        StringSet word[26][26];
        construct(words, len, word);
        auto handler = new MaxWord(head, tail, enable_loop, word, result);
        int ret = handler->handle();
        delete handler;
        return ret;
    }

    static int gen_chains_all(char* words[], int len, char* result[]) {
        StringSet word[26][26];
        construct(words, len, word);
        auto handler = new CountAndList(word, result);
        int ret = handler->handle();
        delete handler;
        return ret;
    }

    static int gen_chain_word_unique(char* words[], int len, char* result[]) {
        StringSet word[26][26];
        construct(words, len, word);
        auto handler = new DistinctInitial(word, result);
        int ret = handler->handle();
        delete handler;
        return ret;
    }

    static int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop) {
        StringSet word[26][26];
        construct(words, len, word);
        auto handler = new MaxLetter(head, tail, enable_loop, word, result);
        int ret = handler->handle();
        delete handler;
        return ret;
    }
    #pragma warning(pop)
};