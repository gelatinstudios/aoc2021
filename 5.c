
#include <stdio.h>

#define Minimum(a, b) ((a) < (b) ? (a) : (b))
#define Maximum(a, b) ((a) > (b) ? (a) : (b))
#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

typedef struct {
    int x1, y1;
    int x2, y2;
} line;

static unsigned grid[1024][1024];

int get_answer(void) {
    int count = 0;
    for (int y = 0; y < ArrayCount(grid); y++) {
        for (int x = 0; x < ArrayCount(grid[y]); x++) {
            if (grid[y][x] >= 2) count++;
        }
    }
    return count;
}

int main(void) {
    static char buffer[4096];
    
    
    static line lines[1024];
    int line_count = 0;
    
    while (fgets(buffer, sizeof(buffer), stdin)) {
        line *l = &lines[line_count++];
        sscanf(buffer, "%d,%d -> %d,%d", &l->x1, &l->y1, &l->x2, &l->y2);
    }
    
    for (int i = 0; i < line_count; i++) {
        line l = lines[i];
        
        if (l.x1 == l.x2) {
            int x = l.x1;
            int min = Minimum(l.y1, l.y2);
            int max = Maximum(l.y1, l.y2);
            for (int y = min; y <= max; y++) {
                grid[y][x]++;
            }
        }
        
        if (l.y1 == l.y2) {
            int y = l.y1;
            int min = Minimum(l.x1, l.x2);
            int max = Maximum(l.x1, l.x2);
            for (int x = min; x <= max; x++) {
                grid[y][x]++;
            }
        }
    }
    
    printf("%d\n", get_answer());
    
    for (int i = 0; i < line_count; i++) {
        line l = lines[i];
        
        if (l.x1 != l.x2 && l.y1 != l.y2) {
            int x = l.x1;
            int y = l.y1;
            
            int dx = 1;
            int dy = 1;
            if (l.x1 > l.x2) dx = -1;
            if (l.y1 > l.y2) dy = -1;
            
            for (;;) {
                grid[y][x]++;
                
                if (x == l.x2) break;
                
                x += dx;
                y += dy;
            }
        }
    }
    
    printf("%d\n", get_answer());
}