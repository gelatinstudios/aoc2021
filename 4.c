
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int boards[128][5][5];
static int marks[128][5][5];
static int board_count = 0;

int check_for_win(int board) {
    for (int i = 0; i < 5; i++) {
        if (marks[board][i][0] &&
            marks[board][i][1] &&
            marks[board][i][2] &&
            marks[board][i][3] &&
            marks[board][i][4]) return 1;
        
        if (marks[board][0][i] &&
            marks[board][1][i] &&
            marks[board][2][i] &&
            marks[board][3][i] &&
            marks[board][4][i]) return 1;
    }
    
    return 0;
}

int main(void) {
    static int numbers[4096];
    int number_count = 0;
    
    static char line[4096];
    
    fgets(line, sizeof(line), stdin);
    
    char *sep = ",\r\n";
    char *tok = strtok(line, sep);
    while (tok) {
        numbers[number_count++] = atoi(tok);
        tok = strtok(0, sep);
    }
    
    while (fgets(line, sizeof(line), stdin)) {
        for (int i = 0; i < 5; i++) {
            fgets(line, sizeof(line), stdin);
            
            sscanf(line, "%d %d %d %d %d", 
                   &boards[board_count][i][0],
                   &boards[board_count][i][1],
                   &boards[board_count][i][2],
                   &boards[board_count][i][3],
                   &boards[board_count][i][4]);
        }
        
        board_count++;
    }
    
    int sum = 0;
    int first_win = 0;
    int last_win = 0;
    
    static unsigned char wins[128];
    
    for (int i = 0; i < number_count; i++) {
        int n = numbers[i];
        
        for (int board = 0; board < board_count; board++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (boards[board][i][j] == n) {
                        marks[board][i][j] = 1;
                    }
                }
            }
            
            if (!wins[board] && check_for_win(board)) {
                int sum_of_unmarked = 0;
                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 5; j++) {
                        if (!marks[board][i][j]) {
                            sum_of_unmarked += (int)boards[board][i][j];
                        }
                    }
                }
                
                int score = (int)sum_of_unmarked * (int)n;
                if (!first_win) {
                    sum = sum_of_unmarked;
                    first_win = score;
                }
                last_win = score;
                wins[board] = 1;
            }
        }
    }
    
    printf("%d\n", first_win);
    printf("%d\n", last_win);
}