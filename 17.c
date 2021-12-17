
#include <stdio.h>
#include <limits.h>

// -300, 300, and 1000 were chosen pretty arbitrarily :)

int main(void) {
    char input[128];
    fgets(input, sizeof(input), stdin);
    
    int min_x, min_y, max_x, max_y;
    sscanf(input, "target area: x=%d..%d, y=%d..%d", &min_x, &max_x, &min_y, &max_y);
    
    int min_idy = INT_MAX;
    int max_idy = INT_MIN;
    
    int highest_y = 0;
    for (int idy = -300; idy <= 300; idy++) {
        int highest_y_here = 0;
        int dy = idy;
        int y = 0;
        while (y > max_y) {
            y += dy;
            if (y > highest_y_here) highest_y_here = y;
            dy -= 1;
        }
        if (y >= min_y && y <= max_y) {
            if (highest_y_here > highest_y) highest_y = highest_y_here;
            if (idy < min_idy) min_idy = idy;
            if (idy > max_idy) max_idy = idy;
        }
    }
    
    printf("%d\n", highest_y);
    
    int min_idx = INT_MAX;
    int max_idx = INT_MIN;
    
    for (int idx = 1; idx <= 1000; idx++) {
        int dx = idx;
        int x = 0;
        while (x < min_x && dx > 0) {
            x += dx;
            if      (dx < 0) dx++;
            else if (dx > 0) dx--;
        }
        if (x >= min_x && x <= max_x) {
            if (idx < min_idx) min_idx = idx;
            if (idx > max_idx) max_idx = idx;
        }
    } 
    
    int count = 0;
    for (int idy = min_idy; idy <= max_idy; idy++) {
        for (int idx = min_idx; idx <= max_idx; idx++) {
            int x = 0;
            int y = 0;
            int dx = idx;
            int dy = idy;
            while (x < max_x && y > min_y) {
                x += dx;
                y += dy;
                dy--;
                if      (dx < 0) dx++;
                else if (dx > 0) dx--;
                
                if (x >= min_x && x <= max_x &&
                    y >= min_y && y <= max_y) {
                    count++;
                    break;
                }
            }
        }
    }
    
    printf("%d\n", count);
}