#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROCESSES 5
#define RESOURCES 3

int available[RESOURCES];
int maximum[PROCESSES][RESOURCES];
int allocated[PROCESSES][RESOURCES];
int need[PROCESSES][RESOURCES];
int safe_sequence[PROCESSES];
int work[RESOURCES];
int finish[PROCESSES];

void calculate_need() {
    printf("Calculating Need Matrix...\n");
    for (int i = 0; i < PROCESSES; i++) {
        for (int j = 0; j < RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocated[i][j];
        }
    }
}

int is_safe() {
    // Initialize work with available resources
    for (int i = 0; i < RESOURCES; i++) {
        work[i] = available[i];
    }
    
    // Initialize finish to false
    for (int i = 0; i < PROCESSES; i++) {
        finish[i] = 0;
    }
    
    int count = 0;
    
    while (count < PROCESSES) {
        int found = 0;
        
        for (int i = 0; i < PROCESSES; i++) {
            if (finish[i] == 0) {
                int can_allocate = 1;
                
                // Check if process i can be satisfied
                for (int j = 0; j < RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = 0;
                        break;
                    }
                }
                
                if (can_allocate) {
                    // Allocate resources to process i
                    for (int j = 0; j < RESOURCES; j++) {
                        work[j] += allocated[i][j];
                    }
                    
                    safe_sequence[count] = i;
                    finish[i] = 1;
                    count++;
                    found = 1;
                }
            }
        }
        
        if (found == 0) {
            return 0; // Unsafe state
        }
    }
    
    return 1; // Safe state
}

void print_matrices() {
    printf("\n========== Allocation Matrix ==========\n");
    printf("Process\t");
    for (int j = 0; j < RESOURCES; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    
    for (int i = 0; i < PROCESSES; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < RESOURCES; j++) {
            printf("%d\t", allocated[i][j]);
        }
        printf("\n");
    }
    
    printf("\n========== Maximum Matrix ==========\n");
    printf("Process\t");
    for (int j = 0; j < RESOURCES; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    
    for (int i = 0; i < PROCESSES; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < RESOURCES; j++) {
            printf("%d\t", maximum[i][j]);
        }
        printf("\n");
    }
    
    printf("\n========== Need Matrix ==========\n");
    printf("Process\t");
    for (int j = 0; j < RESOURCES; j++) {
        printf("R%d\t", j);
    }
    printf("\n");
    
    for (int i = 0; i < PROCESSES; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < RESOURCES; j++) {
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }
    
    printf("\n========== Available Resources ==========\n");
    for (int j = 0; j < RESOURCES; j++) {
        printf("R%d: %d\t", j, available[j]);
    }
    printf("\n");
}

int main() {
    printf("Banker's Algorithm - Deadlock Avoidance\n");
    printf("========================================\n\n");
    
    // Initialize available resources
    available[0] = 10; // Resource 0
    available[1] = 5;  // Resource 1
    available[2] = 7;  // Resource 2
    
    // Initialize maximum demand
    maximum[0][0] = 7; maximum[0][1] = 5; maximum[0][2] = 3;
    maximum[1][0] = 3; maximum[1][1] = 2; maximum[1][2] = 2;
    maximum[2][0] = 9; maximum[2][1] = 0; maximum[2][2] = 2;
    maximum[3][0] = 2; maximum[3][1] = 2; maximum[3][2] = 2;
    maximum[4][0] = 4; maximum[4][1] = 3; maximum[4][2] = 3;
    
    // Initialize allocated resources
    allocated[0][0] = 0; allocated[0][1] = 1; allocated[0][2] = 0;
    allocated[1][0] = 2; allocated[1][1] = 0; allocated[1][2] = 0;
    allocated[2][0] = 3; allocated[2][1] = 0; allocated[2][2] = 2;
    allocated[3][0] = 2; allocated[3][1] = 1; allocated[3][2] = 1;
    allocated[4][0] = 0; allocated[4][1] = 0; allocated[4][2] = 2;
    
    print_matrices();
    
    // Calculate need matrix
    calculate_need();
    
    printf("\n========== Safety Check ==========\n");
    
    if (is_safe()) {
        printf("System is in SAFE state\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < PROCESSES; i++) {
            printf("P%d ", safe_sequence[i]);
        }
        printf("\n");
    } else {
        printf("System is in UNSAFE state\n");
        printf("Deadlock may occur!\n");
    }
    
    return 0;
}
