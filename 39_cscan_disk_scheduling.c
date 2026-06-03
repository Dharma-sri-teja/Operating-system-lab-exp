#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_REQUESTS 20
#define MAX_TRACK 200
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
int compareAsc(const void *a, const void *b) {
    return ((DiskRequest *)a)->trackNo - ((DiskRequest *)b)->trackNo;
}
void cscanScheduling() {
    printf("===== C-SCAN Disk Scheduling Algorithm =====\n");
    printf("(Circular SCAN)\n");
    printf("Initial Head Position: 50\n");
    printf("Disk Track Range: 0-%d\n", diskTrackRange);
    printf("Direction: Low to High (then wrap around)\n\n");
    totalHeadMovement = 0;
    int currentPosition = 50;
    qsort(requestQueue, requestCount, sizeof(DiskRequest), compareAsc);
    DiskRequest lower[MAX_REQUESTS], higher[MAX_REQUESTS];
    int lowerCount = 0, higherCount = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requestQueue[i].trackNo < currentPosition) {
            lower[lowerCount++] = requestQueue[i];
        } else {
            higher[higherCount++] = requestQueue[i];
        }
    }
    printf("%-8s %-12s %-15s %-15s\n", "Order", "Track", "Head Movement", "Total Movement");
    printf("=============================================================\n");
    int order = 1;
    for (int i = 0; i < higherCount; i++) {
        int trackNo = higher[i].trackNo;
        int movement = abs(currentPosition - trackNo);
        totalHeadMovement += movement;
        printf("%-8d %-12d %-15d %-15d\n", order++, trackNo, movement, totalHeadMovement);
        currentPosition = trackNo;
    }
    if (higherCount > 0 || lowerCount > 0) {
        int movement = abs(currentPosition - diskTrackRange);
        totalHeadMovement += movement;
        printf("%-8d %-12d %-15d %-15d (Go to end)\n", order++, diskTrackRange, movement, totalHeadMovement);
        currentPosition = diskTrackRange;
    }
    if (lowerCount > 0) {
        int movement = diskTrackRange - 0; 
        totalHeadMovement += movement;
        currentPosition = 0;
        printf("%-8d %-12d %-15d %-15d (Wrap to start)\n", order++, 0, movement, totalHeadMovement);
        for (int i = 0; i < lowerCount; i++) {
            int trackNo = lower[i].trackNo;
            movement = abs(currentPosition - trackNo);
            totalHeadMovement += movement;
            printf("%-8d %-12d %-15d %-15d\n", order++, trackNo, movement, totalHeadMovement);
            currentPosition = trackNo;
        }
    }
    printf("=============================================================\n");
    printf("Total Head Movements: %d\n", totalHeadMovement);
    printf("Average Head Movement: %.2f\n\n", (float)totalHeadMovement / requestCount);
}
int main() {
    printf("======= C-SCAN Disk Scheduling Algorithm =======\n");
    printf("(Circular Scan / Modified SCAN)\n\n");
    printf("TEST CASE 1: Standard Request Sequence\n");
    printf("======================================\n");
    addRequest(82);
    addRequest(170);
    addRequest(43);
    addRequest(140);
    addRequest(24);
    addRequest(16);
    addRequest(190);
    cscanScheduling();
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
    cscanScheduling();
    requestCount = 0;
    totalHeadMovement = 0;
    printf("\nTEST CASE 3: Clustered Requests\n");
    printf("==============================\n");
    addRequest(100);
    addRequest(110);
    addRequest(120);
    addRequest(30);
    addRequest(40);
    addRequest(50);
    cscanScheduling();
    printf("\n===== C-SCAN Algorithm Characteristics =====\n");
    printf("1. Head moves from one end to the other\n");
    printf("2. Wraps around to beginning without serving\n");
    printf("3. Provides uniform wait times\n");
    printf("4. Better than SCAN for uniform load\n");
    printf("5. Prevents starvation\n");
    printf("6. Fair disk access distribution\n");
    printf("7. More predictable disk latency\n");
    printf("===========================================\n");
    printf("\n===== Comparison with SCAN =====\n");
    printf("SCAN:   Goes both directions from middle\n");
    printf("C-SCAN: Completes one direction, wraps to start\n");
    printf("Effect: More uniform service for all tracks\n");
    printf("================================\n");
    return 0;
}

