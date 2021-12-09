
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Maximum(a, b) ((a) > (b) ? (a) : (b))

static unsigned char grid[128][128];
static unsigned char marks[128][128];

int cmp(const void *A, const void *B) {
    int *a = (int *)A;
    int *b = (int *)B;
    return *b - *a;
}

int find_basin_size(int i, int j) {
    if (marks[i][j]) return 0;
    if (grid[i][j] >= 9) return 0;
    
    marks[i][j] = 1;
    
    int result = 1;
    result += find_basin_size(i+1, j);
    result += find_basin_size(i-1, j);
    result += find_basin_size(i, j+1);
    result += find_basin_size(i, j-1);
    return result;
}

int main(void) {
    memset(grid, 0xff, sizeof(grid));
    
    static int basin_sizes[1024];
    int basin_count = 0;
    
    int column_count = 0;
    int row_count = 1;
    
    static char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        int col = 1;
        for (char *c = line; isdigit(*c); c++) {
            grid[row_count][col++] = *c - '0';
        }
        
        if (!column_count) column_count = col;
        
        row_count++;
    }
    
    int sum = 0;
    for (int i = 1; i < row_count; i++) {
        for (int j = 1; j < column_count; j++) {
            int h = grid[i][j];
            int is_low_point = (h < grid[i+1][j] &&
                                h < grid[i-1][j] &&
                                h < grid[i][j+1] &&
                                h < grid[i][j-1]);
            if (is_low_point) {
                int risk_level = h + 1;
                sum += risk_level;
                
                int basin_size = find_basin_size(i, j);
                basin_sizes[basin_count++] = basin_size;
            }
        }
    }
    
    qsort(basin_sizes, basin_count, sizeof(basin_sizes[0]), cmp);
    
    printf("%d\n", sum);
    printf("%d\n", basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
}