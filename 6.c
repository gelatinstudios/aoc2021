
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t u64;

typedef struct {
    u64 counts[10];
} fish_counts;

u64 sim(fish_counts fish, int days) {
    for (int i = 0; i < days; i++) {
        u64 new_fish = fish.counts[0];
        for (int i = 0; i < 9; i++) {
            fish.counts[i] = fish.counts[i+1];
        }
        fish.counts[6] += new_fish;
        fish.counts[8] = new_fish;
    }
    
    u64 sum = 0;
    for (int i = 0; i < 9; i++) {
        sum += fish.counts[i];
    }
    return sum;
}

int main(void) {
    fish_counts fish = {0};
    
    static char buffer[4096*3];
    fgets(buffer, sizeof(buffer), stdin);
    
    char *sep = ",\n\r";
    char *tok = strtok(buffer, sep);
    while (tok) {
        fish.counts[atoi(tok)]++;
        tok = strtok(0, sep);
    }
    
    printf("%llu\n", sim(fish, 80));
    printf("%llu\n", sim(fish, 256));
}