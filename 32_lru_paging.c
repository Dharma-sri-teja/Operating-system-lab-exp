#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FRAMES 3
#define MAX_PAGES 20
typedef struct {
    int page;
    int lastUsedTime;
} FrameInfo;
FrameInfo frames[MAX_FRAMES];
int pageSequence[MAX_PAGES];
int sequenceLength = 0;
int pageFaults = 0;
int pageHits = 0;
int currentTime = 0;
void initializeFrames() {
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i].page = -1;
        frames[i].lastUsedTime = -1;
    }
}
int findPage(int page) {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i].page == page) {
            return i;
        }
    }
    return -1;
}
int findLRU() {
    int minTime = frames[0].lastUsedTime;
    int minIndex = 0;
    for (int i = 1; i < MAX_FRAMES; i++) {
        if (frames[i].lastUsedTime < minTime) {
            minTime = frames[i].lastUsedTime;
            minIndex = i;
        }
    }
    return minIndex;
}
int isFrameFull() {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i].page == -1) {
            return 0;
        }
    }
    return 1;
}
void insertPage(int page, int time) {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i].page == -1) {
            frames[i].page = page;
            frames[i].lastUsedTime = time;
            return;
        }
    }
    int lruIndex = findLRU();
    frames[lruIndex].page = page;
    frames[lruIndex].lastUsedTime = time;
}
void updatePageTime(int pageIndex, int time) {
    frames[pageIndex].lastUsedTime = time;
}
void displayFrames() {
    printf("Frames: [ ");
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i].page != -1) {
            printf("(%d|%d) ", frames[i].page, frames[i].lastUsedTime);
        }
    }
    printf("]\n");
}
void lruPageReplacement() {
    printf("===== LRU (Least Recently Used) Page Replacement =====\n");
    printf("Number of frames: %d\n\n", MAX_FRAMES);
    initializeFrames();
    pageFaults = 0;
    pageHits = 0;
    currentTime = 0;
    printf("%-5s %-10s %-30s %-15s\n", "Step", "Page", "Frames (page|time)", "Status");
    printf("=================================================================\n");
    for (int i = 0; i < sequenceLength; i++) {
        int page = pageSequence[i];
        printf("%-5d %-10d ", i + 1, page);
        int pageIndex = findPage(page);
        if (pageIndex != -1) {
            pageHits++;
            updatePageTime(pageIndex, currentTime);
            printf("%-30s %-15s\n", "", "HIT");
        } else {
            pageFaults++;
            insertPage(page, currentTime);
            printf("%-30s %-15s\n", "", "FAULT");
        }
        displayFrames();
        currentTime++;
    }
    printf("\n=================================================================\n");
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits: %d\n", pageHits);
    printf("Total Memory References: %d\n", sequenceLength);
    printf("Page Fault Rate: %.2f%%\n", (pageFaults * 100.0) / sequenceLength);
    printf("Page Hit Rate: %.2f%%\n", (pageHits * 100.0) / sequenceLength);
}
int main() {
    printf("======= LRU Page Replacement Technique =======\n\n");
    printf("TEST CASE 1: Standard Page Sequence\n");
    printf("==================================\n");
    int sequence1[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    sequenceLength = 20;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence1[i];
    }
    lruPageReplacement();
    printf("\n\nTEST CASE 2: Different Sequence\n");
    printf("===============================\n");
    int sequence2[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    sequenceLength = 12;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence2[i];
    }
    lruPageReplacement();
    printf("\n\nTEST CASE 3: Working Set Example\n");
    printf("================================\n");
    int sequence3[] = {1, 2, 3, 4, 1, 2, 3, 4, 5, 3, 4, 5, 1, 2, 3};
    sequenceLength = 15;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence3[i];
    }
    lruPageReplacement();
    return 0;
}
