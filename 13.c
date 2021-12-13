
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct { int x, y; } iv2;

typedef struct {
    int pitch;
    int width;
    int height;
    unsigned char *grid;
} paper;

void put_dot(paper p, int x, int y, int value) {
    if (x < p.width && y < p.height) {
        p.grid[y*p.pitch + x] = value;
    }
}

int check_dot(paper p, int x, int y) {
    if (x < p.width && y < p.height) {
        return p.grid[y*p.pitch + x];
    }
    return 0;
}

void fold_paper(paper *p, iv2 v) {
    if (v.x) {
        for (int y = 0; y < p->height; y++) {
            for (int x = v.x+1; x < p->width; x++) {
                int fx = (v.x - (x % v.x)) % v.x;
                put_dot(*p, fx, y, check_dot(*p, x, y) | check_dot(*p, fx, y));
            }
        }
        p->width = v.x;
    } else {
        for (int y = v.y+1; y < p->height; y++) {
            for (int x = 0; x < p->width; x++) {
                int fy = (v.y - (y % v.y)) % v.y;
                put_dot(*p, x, fy, check_dot(*p, x, y) | check_dot(*p, x, fy));
            }
        }
        p->height = v.y;
    }
}

int main(void) {
    static iv2 coords[1024];
    int coord_count = 0;
    
    static iv2 folds[16];
    int fold_count = 0;
    
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        if (isdigit(line[0])) {
            iv2 *v = &coords[coord_count++];
            sscanf(line, "%d,%d", &v->x, &v->y);
        } else if (isalpha(line[0])) {
            char c;
            int n;
            sscanf(line, "fold along %c=%d", &c, &n);
            iv2 fold = {0};
            if (c == 'x') fold.x = n;
            else          fold.y = n;
            folds[fold_count++] = fold;
        }
    }
    
    int max_x = 0;
    int max_y = 0;
    for (int i = 0; i < coord_count; i++) {
        iv2 v = coords[i];
        if (v.x > max_x) max_x = v.x;
        if (v.y > max_y) max_y = v.y;
    }
    
    int width = max_x + 1;
    int height = max_y + 1;
    
    paper p = { width, width, height, (unsigned char *)malloc(width*height) };
    memset(p.grid, 0, width*height);
    
    for (int i = 0; i < coord_count; i++) {
        iv2 v = coords[i];
        put_dot(p, v.x, v.y, 1);
    }
    
    fold_paper(&p, folds[0]);
    
    int dots = 0;
    for (int y = 0; y < p.height; y++) {
        for (int x = 0; x < p.width; x++) {
            dots += check_dot(p, x, y);
        }
    }
    
    printf("%d\n", dots);
    
    for (int i = 1; i < fold_count; i++) {
        fold_paper(&p, folds[i]);
    }
    
    putchar('\n');
    for (int y = 0; y < p.height; y++) {
        for (int x = 0; x < p.width; x++) {
            putchar(check_dot(p, x, y) ? '#' : ' ');
        }
        putchar('\n');
    }
}