
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// x - horizontal
// y - depth

int main(void) {
    int x = 0;
    int y = 0;
    
    int x2 = 0;
    int y2 = 0;
    int aim = 0;
    
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        char *sep = " \n\r\t";
        
        char *direction = strtok(line, sep);
        int n = atoi(strtok(0, sep));
        
        if (!strcmp(direction, "forward")) x += n;
        if (!strcmp(direction, "down"))    y += n;
        if (!strcmp(direction, "up"))      y -= n;
        
        if (!strcmp(direction, "forward")) {
            x2 += n;
            y2 += aim*n;
        }
        if (!strcmp(direction, "down"))    aim += n;
        if (!strcmp(direction, "up"))      aim -= n;
    }
    
    printf("%d\n", x*y);
    printf("%d\n", x2*y2);
}