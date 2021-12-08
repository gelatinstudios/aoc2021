
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define N 1024

unsigned char bits[N][12];
int line_count = 0;

int find_most_common_bit_in_column_mask(int column, unsigned char *mask) {
    int ones = 0;
    int count = 0;
    for (int line = 0; line < line_count; line++) {
        if (mask[line]) {
            ones += bits[line][column];
            count++;
        }
    }
    int zeroes = count - ones;
    if (ones == zeroes) return -1;
    return ones > zeroes;
}

unsigned find_most_common_bit_in_column(int column) {
    unsigned char mask[N];
    memset(mask, 1, sizeof(mask));
    return find_most_common_bit_in_column_mask(column, mask);
}

// 1 - oxygen
// 0 - CO2 scrubber
unsigned find_value(int criteria) {
    unsigned char mask[N];
    memset(mask, 1, sizeof(mask));
    
    int mask_count = line_count;
    
    int bit = 0;
    int found = 0;
    while (!found) {
        int keep = -1;
        
        int most_common = find_most_common_bit_in_column_mask(bit, mask);
        if (most_common == -1) {
            keep = criteria;
        } else {
            keep = criteria == most_common;
        }
        
        assert(keep >= 0);
        
        for (int line = 0; line < line_count; line++) {
            if (mask[line] && bits[line][bit] != keep) {
                mask[line] = 0;
                mask_count--;
                
                if (mask_count == 1) {
                    found = 1;
                    break;
                }
            }
        }
        
        bit++;
    }
    
    for (int line = 0; line < line_count; line++) {
        if (mask[line]) {
            unsigned result = 0;
            for (int bit = 0; bit < 12; bit++) {
                result |= bits[line][bit] << (11 - bit);
            }
            return result;
        }
    }
    
    return 0;
}

int main(void) {
    char line[4096];
    
    while (fgets(line, sizeof(line), stdin)) {
        for (int i = 0; i < 12; i++) 
            bits[line_count][i] = line[i] - '0';
        line_count++;
    }
    
    unsigned gamma = 0;
    unsigned epsilon = 0;
    
    for (int bit = 0; bit < 12; bit++) {
        unsigned gamma_bit = find_most_common_bit_in_column(bit);
        unsigned epsilon_bit = !gamma_bit;
        
        gamma |= gamma_bit << (11 - bit);
        epsilon |= epsilon_bit << (11 - bit);
    }
    
    printf("%u\n", gamma * epsilon);
    
    printf("%d\n", find_value(1) * find_value(0));
}