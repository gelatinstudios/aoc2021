
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char line[4096];
    int measurements[2048];
    int measurement_count = 0;
    
    while (fgets(line, sizeof(line), stdin)) {
        measurements[measurement_count++] = atoi(line);
    }
    
    int increase_count = 0;
    for (int i = 1; i < measurement_count; i++) {
        increase_count += measurements[i] > measurements[i-1];
    }
    printf("%d\n", increase_count);
    
    int increase_count2 = 0;
    for (int i = 3; i < measurement_count; i++) {
        int a = measurements[i-3];
        int b = measurements[i-2];
        int c = measurements[i-1];
        int d = measurements[i-0];
        
        int sum1 = a + b + c;
        int sum2 = b + c + d;
        
        increase_count2 += sum2 > sum1;
    }
    
    printf("%d\n", increase_count2);
}