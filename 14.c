
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef int64_t s64;

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

static s64 polymer[128][128] = {0};
static s64 counts[128] = {0};
static s64 tmp[128][128] = {0};
static char rules[128][128] = {0};

void n_steps(int N) {
    for (int n = 0; n < N; n++) {
        memcpy(tmp, polymer, sizeof(polymer));
        memset(polymer, 0, sizeof(polymer));
        
        for (int i = 0; i < 128; i++) {
            for (int j = 0; j < 128; j++) {
                char r = rules[i][j];
                s64 n = tmp[i][j];
                if (r) {
                    polymer[i][r] += n;
                    polymer[r][j] += n;
                    counts[r] += n;
                } else {
                    polymer[i][j] += n;
                }
            }
        }
    }
}

s64 get_result(void) {
    s64 min = INT64_MAX;
    s64 max = 0;
    for (int i =0; i < ArrayCount(counts); i++) {
        s64 n = counts[i];
        if (n > 0) {
            if (n < min) min = n;
            if (n > max) max = n;
        }
    }
    return max - min;
}

int main(void) {
    char line [4096];
    
    fgets(line, sizeof(line), stdin);
    
    counts[line[0]]++;
    for (char *c = line+1; isalpha(*c); c++) {
        polymer[c[-1]][c[0]]++;
        counts[c[0]]++;
    }
    
    fgets(line, sizeof(line), stdin); // blank line
    
    while (fgets(line, sizeof(line), stdin)) {
        char a, b, c;
        sscanf(line, "%c%c -> %c", &a, &b, &c);
        rules[a][b] = c;
    }
    
    n_steps(10);
    printf("%lld\n", get_result());
    n_steps(30);
    printf("%lld\n", get_result());
}