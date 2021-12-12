
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *clone(char *s) {
    size_t n = strlen(s) + 1;
    char *result = malloc(n);
    memcpy(result, s, n);
    return result;
}

#define N 32

static int cave_count = 0;
static char *names[N];
static unsigned char big_caves[N] = {0};
static unsigned char connections[N][N] = {0};

static int start_index = -1;
static int end_index = -1;

int find_or_add_cave(char *name) {
    for (int i = 0; i < cave_count; i++) {
        if (!strcmp(name, names[i])) {
            return i;
        }
    }
    
    int index = cave_count;
    
    if (!strcmp(name, "start")) start_index = index;
    if (!strcmp(name, "end")) end_index = index;
    
    int all_upper = 1;
    for (char *c = name; *c; c++) {
        if (!isupper(*c)) {
            all_upper = 0;
            break;
        }
    }
    if (all_upper) big_caves[index] = 1;
    
    names[cave_count++] = clone(name);
    return index;
}

typedef struct { unsigned char tab[N]; } visited_table;

int count_paths(int index, visited_table v, int extra_small_used) {
    if (index == end_index) return 1;
    if (!big_caves[index]) v.tab[index]++;
    int result = 0;
    for (int i = 0; i < cave_count; i++) {
        if (connections[index][i]) {
            if (v.tab[i]) {
                if (!extra_small_used && i != start_index) {
                    result += count_paths(i, v, 1);
                }
            } else {
                result += count_paths(i, v, extra_small_used);
            }
        }
    }
    return result;
}

int main(void) {
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        char *sep = "-\r\n\t ";
        
        int a = find_or_add_cave(strtok(line, sep));
        int b = find_or_add_cave(strtok(0, sep));
        
        connections[a][b] = 1;
        connections[b][a] = 1;
    }
    
    assert(start_index > 0);
    assert(end_index > 0);
    
    visited_table v = {0};
    printf("%d\n", count_paths(start_index, v, 1));
    printf("%d\n", count_paths(start_index, v, 0));
}