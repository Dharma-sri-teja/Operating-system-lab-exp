#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_REQUESTS 20
typedef struct {
    int trackNo;
    int processed;
} DiskRequest;
DiskRequest requestQueue[MAX_REQUESTS];
int requestCount = 0;
int totalHeadMovement = 0;
int diskTrackRange = 200;
void addRequest(int trackNo) {
    if (requestCount < MAX_REQUESTS) {
        requestQueue[requestCount].trackNo = trackNo;
        requestQueue[requestCount].processed = 0;
        requestCount++;
    }
}
void fcfsScheduling() {
    printf("===== FCFS Disk Scheduling Algorithm =====\n");
    printf("Initial Head Position: 50\n");
    printf("Disk Track Range: 0-%d\n\n", diskTrackRange);
    totalHeadMovement = 0;
    int currentPosition = 50;
    printf("%-8s %-12s %-15s %-15s\n", "Order", "Track", "Head Movement", "Total Movement");
    printf("=============================================================\n");
    for (int i = 0; i < requestCount; i++) {
        int trackNo = requestQueue[i].trackNo;
        int movement = abs(currentPosition - trackNo);
        totalHeadMovement += movement;
        printf("%-8d %-12d %-15d %-15d\n", i + 1, trackNo, movement, totalHeadMovement);
        currentPosition = trackNo;
    }
    printf("=============================================================\n");
    printf("Total Head Movements: %d\n", totalHeadMovement);
    printf("Average Head Movement: %.2f\n\n", (float)totalHeadMovement / requestCount);
}
int main() {
    printf("======= FCFS Disk Scheduling Algorithm =======\n");
    printf("(First Come First Served)\n\n");
    printf("TEST CASE 1: Standard Request Sequence\n");
    printf("======================================\n");
    addRequest(82);
    addRequest(170);
    addRequest(43);
    addRequest(140);
    addRequest(24);
    addRequest(16);
    addRequest(190);
    fcfsScheduling();
    requestCount = 0;
    totalHeadMovement = 0;
    printf("\nTEST CASE 2: Different Request Sequence\n");
    printf("=======================================\n");
    addRequest(95);
    addRequest(180);
    addRequest(34);
    addRequest(119);
    addRequest(11);
    addRequest(123);
    fcfsScheduling();
    requestCount = 0;
    totalHeadMovement = 0;
    printf("\nTEST CASE 3: Random Request Pattern\n");
    printf("===================================\n");
    addRequest(100);
    addRequest(50);
    addRequest(150);
    addRequest(25);
    addRequest(175);
    addRequest(10);
    fcfsScheduling();
    printf("\n===== FCFS Algorithm Characteristics =====\n");
    printf("1. Processes requests in arrival order\n");
    printf("2. Simple to implement\n");
    printf("3. Fair to all processes\n");
    printf("4. Can cause high head movement\n");
    printf("5. May not optimize seek time\n");
    printf("6. Can cause convoy effect\n");
    printf("==========================================\n");
    return 0;
}
