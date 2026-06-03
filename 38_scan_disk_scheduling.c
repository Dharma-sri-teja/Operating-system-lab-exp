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
void scanScheduling() {
    printf("===== SCAN Disk Scheduling Algorithm =====\n");
    printf("Initial Head Position: 50\n");
    printf("Disk Track Range: 0-%d\n", diskTrackRange);
    printf("Direction: Low to High\n\n");
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
    for (int i = lowerCount - 1; i >= 0; i--) {
        int trackNo = lower[i].trackNo;
        int movement = abs(currentPosition - trackNo);
        totalHeadMovement += movement;
        printf("%-8d %-12d %-15d %-15d\n", order++, trackNo, movement, totalHeadMovement);
        currentPosition = trackNo;
    }
    printf("=============================================================\n");
    printf("Total Head Movements: %d\n", totalHeadMovement);
    printf("Average Head Movement: %.2f\n\n", (float)totalHeadMovement / requestCount);
}
int main() {
    printf("======= SCAN Disk Scheduling Algorithm =======\n");
    printf("(Elevator Algorithm)\n\n");
    printf("TEST CASE 1: Standard Request Sequence\n");
    printf("======================================\n");
    addRequest(82);
    addRequest(170);
    addRequest(43);
    addRequest(140);
    addRequest(24);
    addRequest(16);
    addRequest(190);
    scanScheduling();
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
    scanScheduling();
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
    scanScheduling();
    printf("\n===== SCAN Algorithm Characteristics =====\n");
    printf("1. Head moves in one direction till end\n");
    printf("2. Then reverses direction\n");
    printf("3. Prevents starvation\n");
    printf("4. Better than FCFS for most cases\n");
    printf("5. Head movement bounded\n");
    printf("6. No convoy effect\n");
    printf("7. Fair disk access\n");
    printf("=========================================\n");
    return 0;
}
