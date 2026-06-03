#include <stdio.h>
#include <stdlib.h>
#define MAX_FRAMES 3
#define MAX_PAGES 20
typedef struct {
    int pages[MAX_FRAMES];
    int front;
    int rear;
    int count;
} PageFrame;
PageFrame frames;
int pageSequence[MAX_PAGES];
int sequenceLength = 0;
int pageFaults = 0;
int pageHits = 0;
void initializeFrames() {
    frames.front = 0;
    frames.rear = 0;
    frames.count = 0;
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames.pages[i] = -1;
    }
}
int isPageInFrame(int page) {
    for (int i = 0; i < frames.count; i++) {
        if (frames.pages[i] == page) {
            return 1;
        }
    }
    return 0;
}
void insertPage(int page) {
    if (frames.count < MAX_FRAMES) {
        frames.pages[frames.count] = page;
        frames.count++;
    } else {
        frames.pages[frames.front] = page;
        frames.front = (frames.front + 1) % MAX_FRAMES;
    }
}
void displayFrames() {
    printf("Frames: [ ");
    for (int i = 0; i < frames.count; i++) {
        printf("%d ", frames.pages[i]);
    }
    printf("]\n");
}
void fifoPageReplacement() {
    printf("===== FIFO Page Replacement Algorithm =====\n");
    printf("Number of frames: %d\n\n", MAX_FRAMES);
    initializeFrames();
    pageFaults = 0;
    pageHits = 0;
    printf("%-5s %-10s %-25s %-15s\n", "Step", "Page", "Frames", "Status");
    printf("============================================================\n");
    for (int i = 0; i < sequenceLength; i++) {
        int page = pageSequence[i];
        printf("%-5d %-10d ", i + 1, page);
        if (isPageInFrame(page)) {
            pageHits++;
            printf("%-25s %-15s\n", "", "HIT");
        } else {
            pageFaults++;
            insertPage(page);
            printf("%-25s %-15s\n", "", "FAULT");
        }
        displayFrames();
    }
    printf("\n============================================================\n");
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits: %d\n", pageHits);
    printf("Total Memory References: %d\n", sequenceLength);
    printf("Page Fault Rate: %.2f%%\n", (pageFaults * 100.0) / sequenceLength);
    printf("Page Hit Rate: %.2f%%\n", (pageHits * 100.0) / sequenceLength);
}
int main() {
    printf("======= FIFO Page Replacement Technique =======\n\n");
    printf("TEST CASE 1: Simple Page Sequence\n");
    printf("================================\n");
    int sequence1[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    sequenceLength = 20;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence1[i];
    }
    fifoPageReplacement();
    printf("\n\nTEST CASE 2: Different Page Sequence\n");
    printf("====================================\n");
    int sequence2[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    sequenceLength = 12;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence2[i];
    }
    fifoPageReplacement();
    printf("\n\nTEST CASE 3: Belady's Anomaly Example\n");
    printf("====================================\n");
    int sequence3[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 1, 2, 3, 4, 5, 4, 3, 2, 1};
    sequenceLength = 20;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence3[i];
    }
    fifoPageReplacement();
    return 0;
}
