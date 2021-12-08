
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int position_table[4096] = {0};

int triangle(int n) { return n*(n+1)/2; }

int find_best_position(int min_position, int max_position, int use_triangle) {
    int least_fuel = INT_MAX;
    for (int i = min_position; i <= max_position; i++) {
        int fuel = 0;
        for (int j = min_position; j <= max_position; j++) {
            int d = abs(j-i);
            fuel += position_table[j] * (use_triangle ? triangle(d) : d);
        }
        if (fuel < least_fuel) least_fuel = fuel;
    }
    return least_fuel;
}

int main(void) {
    static char buffer[4096*3];
    fgets(buffer, sizeof(buffer), stdin);
    
    int min_position = INT_MAX;
    int max_position = 0;
    
    char *sep = ",\n\r";
    char *tok = strtok(buffer, sep);
    while (tok) {
        int position = atoi(tok);
        if (position > max_position) max_position = position;
        if (position < min_position) min_position = position;
        position_table[position]++;
        tok = strtok(0, sep);
    }
    
    printf("%d\n", find_best_position(min_position, max_position, 0));
    printf("%d\n", find_best_position(min_position, max_position, 1));
}