#include "util.h"

#include <cstdlib>

int parseNumber(const char *s, size_t *pos, timestamp *n) {
    int i = (pos == NULL ? 0 : *pos);
    timestamp ans = 0;
    if (!('0' <= s[i] && s[i] <= '9'))
        return 1;
    while ('0' <= s[i] && s[i] <= '9') {
        int d = s[i++] - '0';
        if (ans > MAX_TIMESTAMP / 10)
            return 1;
        ans *= 10;
        if (ans >= MAX_TIMESTAMP - d)
            return 1;
        ans += d;
    }
    if (pos != NULL)
        *pos = i;
    *n = ans;
    return 0;
}
