
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t u64;

int check_match(char a, char b) {
    if (a == '{' && b == '}') return 1;
    if (a == '(' && b == ')') return 1;
    if (a == '[' && b == ']') return 1;
    if (a == '<' && b == '>') return 1;
    return 0;
}

int cmp(const void *A, const void *B) {
    u64 a = *(u64 *)A;
    u64 b = *(u64 *)B;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int main(void) {
    char line[4096];
    
    static u64 incompletion_scores[4096];
    int incomplete_count = 0;
    
    int score = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        char stack[4096];
        int stack_count = 0;
        
        int is_corrupt = 0;
        for (char *c = line; *c; c++) {
            if (is_corrupt) break;
            switch (*c) {
                case '(':
                case '[':
                case '{':
                case '<': {
                    stack[stack_count++] = *c;
                } break;
                
                case ')':
                case ']':
                case '}':
                case '>': {
                    if (stack_count) {
                        char k = stack[--stack_count];
                        if (!check_match(k, *c)) {
                            switch (*c) {
                                case ')': score += 3;     break;
                                case ']': score += 57;    break;
                                case '}': score += 1197;  break;
                                case '>': score += 25137; break;
                            }
                            
                            is_corrupt = 1;
                        }
                    }
                } break;
            }
        }
        
        if (!is_corrupt && stack_count > 0) { // must be incomplete
            u64 incompletion_score = 0;
            while (stack_count > 0) {
                incompletion_score *= 5;
                char k = stack[--stack_count];
                switch (k) {
                    case '(': incompletion_score += 1; break;
                    case '[': incompletion_score += 2; break;
                    case '{': incompletion_score += 3; break;
                    case '<': incompletion_score += 4; break;
                }
            }
            incompletion_scores[incomplete_count++] = incompletion_score;
        }
    }
    
    printf("%d\n", score);
    
    qsort(incompletion_scores, incomplete_count, sizeof(incompletion_scores[0]), cmp);
    
    printf("%llu\n", incompletion_scores[incomplete_count/2]);
}