#ifndef __COMMON_H__
#define __COMMON_H__

#include "trie.h"
#include "util.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <exception>
#include <fcntl.h>
#include <optional>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
using namespace std;

struct EOL : std::exception {};

struct Event {
    size_t pos;
    timestamp ts;
    int tp;
    int eof;

    int c;
    int ap_cnt;
    vector<int> ap_lookup;

    Event(int ap_cnt = 0) : pos(0), ts(0), tp(-1), eof(0), c(-1), ap_cnt(ap_cnt), ap_lookup(ap_cnt) {}
    Event(const Event *e)
        : pos(e->pos), ts(e->ts), tp(e->tp), eof(e->eof), c(e->c), ap_cnt(e->ap_cnt), ap_lookup(e->ap_lookup) {}
    bool operator<(const Event &e) const { return c < e.c || (c == e.c && ap_lookup < e.ap_lookup); }
    Event *clone() const { return new Event(this); }
    int eval(int fid) const {
        CHECK(0 <= fid && fid < ap_cnt);
        return ap_lookup[fid];
    }
    int evalPred(const char *pred_name, int pred) const {
        if (c == -1) {
            CHECK(0 <= pred && pred < ap_cnt);
            return ap_lookup[pred];
        } else {
            return c == pred_name[0];
        }
    }
};

#endif /* __COMMON_H__ */
