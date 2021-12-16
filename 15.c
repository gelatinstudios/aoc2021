
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

// i did the trick of padding the input to avoid having to bounds-check
// this messed up how i kept track of the width and height of the grid,
// so row_count and col_count are kinda all over the place. sorry.

// anyways this is just an ad-hoc implementation of A*

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

#define N 640

static unsigned char grid[N][N];

typedef struct {
    int x, y;
} iv2;

int iv2_eq(iv2 a, iv2 b) {
    return a.x == b.x && a.y == b.y;
}

static iv2 came_from[N][N];
static int g_scores[N][N];
static int f_scores[N][N];

static iv2 end;

#define IV2Index(v) v.y][v.x

int g(iv2 v) {
    return g_scores[IV2Index(v)];
}

int h(iv2 v) {
    return abs(v.x - end.x) + abs(v.y - end.y) ;
}

int f(iv2 v) {
    return f_scores[IV2Index(v)];
}

int a_star(int col_count, int row_count) {
    static iv2 open_set[N*N];
    int open_count = 0;
    
    iv2 start = {1,1};
    end.x = col_count-1;
    end.y = row_count-1;
    
    open_set[open_count++] = start;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            f_scores[i][j] = N*N;
            g_scores[i][j] = N*N;
        }
    }
    
    g_scores[1][1] = 0;
    f_scores[1][1] = h(start);
    
    iv2 neighbor_offsets[] = {
        0, 1,
        0, -1,
        1, 0,
        -1, 0
    };
    
    static iv2 path[N*N];
    int path_count = 0;
    
    while (open_count > 0) {
        int lowest_index =0;
        int lowest_f_score = INT_MAX;
        for (int i = 0; i < open_count; i++) {
            iv2 node = open_set[i];
            int f_score = f(node);
            if (f_score < lowest_f_score) {
                lowest_f_score = f_score;
                lowest_index = i;
            }
        }
        
        iv2 current = open_set[lowest_index];
        if (iv2_eq(current, end)) {
            // goal reached
            
            path[path_count++] = current;
            for (;;) {
                iv2 c = came_from[IV2Index(current)];
                if (c.x == 0 && c.y == 0) break;
                
                current = c;
                path[path_count++] = c;
            }
            break;
        }
        
        for (int i = lowest_index; i < open_count-1; i++) {
            open_set[i] = open_set[i+1];
        }
        open_count--;
        
        for (int i = 0; i < ArrayCount(neighbor_offsets); i++) {
            iv2 offsets = neighbor_offsets[i];
            iv2 neighbor = {current.x + offsets.x, current.y + offsets.y};
            
            if (grid[IV2Index(neighbor)] != 255) {
                int tentative_g_score = g(current) + grid[IV2Index(neighbor)];
                if (tentative_g_score < g(neighbor)) {
                    came_from[IV2Index(neighbor)] = current;
                    g_scores[IV2Index(neighbor)] = tentative_g_score;
                    f_scores[IV2Index(neighbor)] = tentative_g_score + h(neighbor);
                    
                    int found_neighbor = 0;
                    for (int i = 0; i < open_count; i++) {
                        iv2 node = open_set[i];
                        if (iv2_eq(node, neighbor)) {
                            found_neighbor = 1;
                            break;
                        }
                    }
                    
                    if (!found_neighbor) {
                        open_set[open_count++] = neighbor;
                    }
                }
            }
        }
    }
    
    int sum = 0;
    for (int i = 0; i < path_count-1; i++) {
        sum += grid[IV2Index(path[i])];
    }
    
    return sum;
}

void clone_grid(int x_offset, int y_offset, int col_count, int row_count, int addend) {
    for (int y = 1; y < row_count+1; y++) {
        for (int x = 1; x < col_count+1; x++) {
            int n = grid[y][x] + addend;
            while (n > 9) n -= 9;
            grid[y + y_offset][x + x_offset] = n;
        }
    }
}

int main(void) {
    char line[4096];
    
    memset(grid, 0xff, sizeof(grid));
    
    int row_count = 1;
    int col_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        int col = 1;
        for (char *c = line; isdigit(*c); c++) {
            grid[row_count][col++] = *c - '0';
        }
        row_count++;
        if (!col_count) col_count = col;
    }
    
    printf("%d\n", a_star(col_count, row_count));
    
    col_count--;
    row_count--;
    
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            if (x || y) {
                clone_grid(x*col_count, y*row_count, col_count, row_count, x+y);
            }
        }
    }
    
    col_count *= 5;
    row_count *= 5;
    
    col_count++;
    row_count++;
    
    
    printf("%d\n", a_star(col_count, row_count));
}