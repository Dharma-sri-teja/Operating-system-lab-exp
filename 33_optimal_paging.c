#include <stdio.h>
#include <stdlib.h>
#define MAX_FRAMES 3
#define MAX_PAGES 20
int frames[MAX_FRAMES];
int pageSequence[MAX_PAGES];
int sequenceLength = 0;
int pageFaults = 0;
int pageHits = 0;
void initializeFrames() {
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i] = -1;
    }
}
int isPageInFrame(int page) {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i] == page) {
            return 1;
        }
    }
    return 0;
}
int findPageIndex(int page) {
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i] == page) {
            return i;
        }
    }
    return -1;
}
int nextUse(int pageIndex, int currentPos) {
    int nextUseTime = sequenceLength;
    for (int i = currentPos + 1; i < sequenceLength; i++) {
        if (pageSequence[i] == frames[pageIndex]) {
            nextUseTime = i;
            break;
        }
    }
    return nextUseTime;
}
int findOptimal(int currentPos) {
    int maxTime = -1;
    int pageToReplace = 0;
    for (int i = 0; i < MAX_FRAMES; i++) {
        int nextUseTime = nextUse(i, currentPos);
        if (nextUseTime > maxTime) {
            maxTime = nextUseTime;
            pageToReplace = i;
        }
    }
    return pageToReplace;
}
void displayFrames() {
    printf("Frames: [ ");
    for (int i = 0; i < MAX_FRAMES; i++) {
        if (frames[i] != -1) {
            printf("%d ", frames[i]);
        }
    }
    printf("]\n");
}
void optimalPageReplacement() {
    printf("===== Optimal Page Replacement Algorithm =====\n");
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
            int emptyFrame = -1;
            for (int j = 0; j < MAX_FRAMES; j++) {
                if (frames[j] == -1) {
                    emptyFrame = j;
                    break;
                }
            }
            if (emptyFrame != -1) {
                frames[emptyFrame] = page;
            } else {
                int optimalIndex = findOptimal(i);
                frames[optimalIndex] = page;
            }
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
    printf("======= Optimal Page Replacement Technique =======\n\n");
    printf("TEST CASE 1: Classic Optimal Example\n");
    printf("====================================\n");
    int sequence1[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    sequenceLength = 20;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence1[i];
    }
    optimalPageReplacement();
    printf("\n\nTEST CASE 2: Simple Sequence\n");
    printf("============================\n");
    int sequence2[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    sequenceLength = 12;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence2[i];
    }
    optimalPageReplacement();
    printf("\n\nTEST CASE 3: Demonstrating Optimality\n");
    printf("====================================\n");
    int sequence3[] = {1, 2, 3, 4, 5, 2, 1, 5, 2, 4, 3, 5, 1, 2, 3};
    sequenceLength = 15;
    for (int i = 0; i < sequenceLength; i++) {
        pageSequence[i] = sequence3[i];
    }
    optimalPageReplacement();
    printf("\n\n=== Algorithm Comparison Notes ===\n");
    printf("Optimal algorithm always produces the minimum number of page faults\n");
    printf("but is impractical as it requires future knowledge of page references.\n");
    printf("It serves as a theoretical lower bound for comparison with other algorithms.\n");
    return 0;
}
