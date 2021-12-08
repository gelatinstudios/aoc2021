
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

typedef uint64_t u64;

typedef struct { 
    int count;
    char segments[16]; 
} pattern;

typedef struct {
    int signal_count;
    int output_count;
    pattern signals[32];
    pattern outputs[32];
} display;

pattern make_pattern(char *s) {
    pattern result = {0};
    result.count = strlen(s);
    memcpy(result.segments, s, result.count);
    return result;
}

int parse_patterns(char *c, pattern *patterns) {
    int count = 0;
    char *sep = " \r\n\t";
    char *tok = strtok(c, sep);
    while (tok) {
        patterns[count++] = make_pattern(tok);
        tok = strtok(0, sep);
    }
    return count;
}

void find_obvious(pattern *code_patterns, pattern *patterns, int count) {
    for (int i = 0; i < count; i++) {
        if (patterns[i].count == 2) code_patterns[1] = patterns[i];
        if (patterns[i].count == 4) code_patterns[4] = patterns[i];
        if (patterns[i].count == 3) code_patterns[7] = patterns[i];
        if (patterns[i].count == 7) code_patterns[8] = patterns[i];
    }
}

int shared_segments(pattern a, pattern b) {
    int result = 0;
    for (int i = 0; i < a.count; i++) {
        for (int j = 0; j < b.count; j++) {
            result += a.segments[i] == b.segments[j];
        }
    }
    return result;
}

typedef struct {
    int n[2];
} shared_entry;

shared_entry find_9[] = {
    1, 2,
    4, 4,
    7, 3,
    8, 6
};

shared_entry find_6[] = {
    1, 1,
    4, 3,
    7, 2,
    8, 6
};

shared_entry find_5[] = {
    1, 1,
    4, 3,
    6, 5,
    7, 2,
    8, 5,
    9, 5,
};

shared_entry find_3[] = {
    1, 2,
    4, 3,
    5, 4,
    6, 4,
    7, 3,
    8, 5,
    9, 5,
};

shared_entry find_2[] = {
    1, 1,
    3, 4,
    4, 2,
    5, 3,
    6, 4,
    7, 2,
    8, 5,
    9, 4,
};

shared_entry find_0[] = {
    1, 2,
    2, 4,
    3, 4,
    4, 3,
    5, 4,
    6, 5,
    7, 3,
    8, 6,
    9, 5,
};

typedef struct {
    int n;
    int count;
    int shared_count;
    shared_entry *shareds;
} find_entry;

#define AddEntry(c, f) {f, c, ArrayCount(find_##f), find_##f}

find_entry find_table[] = {
    AddEntry(6, 9),
    AddEntry(6, 6),
    AddEntry(5, 5),
    AddEntry(5, 3),
    AddEntry(5, 2),
    AddEntry(6, 0)
};

int test_n(pattern *code_patterns, pattern p, find_entry f) {
    if (p.count != f.count) return 0;
    
    for (int i = 0; i < f.shared_count; i++) {
        if (shared_segments(p, code_patterns[f.shareds[i].n[0]]) != f.shareds[i].n[1])
            return 0;
    }
    
    return 1;
}

void find_n(pattern *code_patterns, pattern *patterns, int count, find_entry f) {
    for (int i = 0; i < count; i++) {
        pattern p = patterns[i];
        if (test_n(code_patterns, p, f)) {
            code_patterns[f.n] = p;
            break;
        }
    }
}

int main(void) {
    static char line[4096];
    
    static display displays[256];
    int display_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        char *c = line;
        while (*c != '|') c++;
        *c = 0;
        
        display *d = &displays[display_count++];
        d->signal_count = parse_patterns(line, d->signals);
        d->output_count = parse_patterns(c + 2, d->outputs);
    }
    
    int count = 0;
    for (int i = 0; i < display_count; i++) {
        display d = displays[i];
        
        for (int i = 0; i < d.output_count; i++) {
            int segment_count = d.outputs[i].count;
            if (segment_count != 5 && segment_count != 6) count++;
        }
    }
    
    printf("%d\n", count);
    
    u64 sum = 0;
    for (int i = 0; i < display_count; i++) {
        display d = displays[i];
        
        // set codes
        pattern code_patterns[10] = {0};
        
        find_obvious(code_patterns, d.signals, d.signal_count);
        find_obvious(code_patterns, d.outputs, d.output_count);
        
        for (int i = 0; i < ArrayCount(find_table); i++) {
            find_n(code_patterns, d.signals, d.signal_count, find_table[i]);
            find_n(code_patterns, d.outputs, d.output_count, find_table[i]);
        }
        
        int n = 0;
        for (int i = 0; i < d.output_count; i++) {
            pattern p = d.outputs[i];
            
            int m = -1;
            for (int i = 0; i < 10; i++) {
                pattern cp = code_patterns[i];
                if (p.count == cp.count && shared_segments(p, cp) == p.count) {
                    m = i;
                    break;
                }
            }
            
            assert(m >= 0);
            
            n *= 10;
            n += m;
        }
        
        sum += n;
    }
    
    printf("%llu\n", sum);
}