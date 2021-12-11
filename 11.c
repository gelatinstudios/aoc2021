
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int grid[16][16];
unsigned char flashed[16][16];

void flash(int,int);

int64_t flashes = 0;

void maybe_flash(int i, int j) {
    if (grid[i][j] > 9 && !flashed[i][j]) flash(i, j);
}

void flash(int i, int j) {
    flashed[i][j] = 1;
    
    flashes++;
    
    grid[i+1][j]++;
    grid[i-1][j]++;
    grid[i][j+1]++;
    grid[i][j-1]++;
    grid[i+1][j+1]++;
    grid[i+1][j-1]++;
    grid[i-1][j+1]++;
    grid[i-1][j-1]++;
    
    maybe_flash(i+1,j);
    maybe_flash(i-1,j);
    maybe_flash(i,j+1);
    maybe_flash(i,j-1);
    maybe_flash(i+1,j+1);
    maybe_flash(i+1,j-1);
    maybe_flash(i-1,j+1);
    maybe_flash(i-1,j-1);
}

int main(void) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            grid[i][j] = INT_MIN;
        }
    }
    
    int column_count = 0;
    int row_count = 1;
    
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        int column = 1;
        for (char *c = line; *c; c++) {
            if (isdigit(*c)) {
                grid[row_count][column++] = *c - '0';
            }
        }
        if (!column_count) column_count = column;
        row_count++;
    }
    
    for (int step = 1; ; step++) {
        memset(flashed, 0, sizeof(flashed));
        for (int i = 1; i < row_count; i++) {
            for (int j = 0; j < column_count; j++) {
                grid[i][j]++;
            }
        }
        
        for (int i = 1; i < row_count; i++) {
            for (int j = 1; j < column_count; j++) {
                maybe_flash(i, j);
            }
        }
        
        for (int i = 1; i < row_count; i++) {
            for (int j = 1; j < column_count; j++) {
                if (flashed[i][j]) grid[i][j] = 0;
            }
        }
        
        if (step == 100) {
            printf("%lld\n", flashes);
        }
        
        int all_flashed = 1;
        for (int i = 1; i < row_count; i++) {
            for (int j = 1; j < column_count; j++) {
                if (!flashed[i][j]) {
                    all_flashed = 0;
                    break;
                }
            }
        }
        
        if (all_flashed) {
            printf("%d\n", step);
            break;
        }
    }
}