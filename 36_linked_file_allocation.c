#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BLOCKS 100
#define MAX_FILES 10
#define MAX_FILENAME 50
typedef struct {
    int blockNo;
    int nextBlock;
} BlockNode;
typedef struct {
    int fileId;
    char fileName[MAX_FILENAME];
    int fileSize;
    int firstBlock;
    int lastBlock;
    int blockCount;
} File;
typedef struct {
    int blockNo;
    int isFree;
    int fileId;
    int nextBlock;
} Block;
File fileTable[MAX_FILES];
Block diskBlocks[MAX_BLOCKS];
int fileCount = 0;
int nextFreeBlock = 0;
void initializeDisk() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        diskBlocks[i].blockNo = i;
        diskBlocks[i].isFree = 1;
        diskBlocks[i].fileId = -1;
        diskBlocks[i].nextBlock = -1;
    }
    nextFreeBlock = 0;
}
int allocateBlock(int fileId) {
    for (int i = nextFreeBlock; i < MAX_BLOCKS; i++) {
        if (diskBlocks[i].isFree) {
            diskBlocks[i].isFree = 0;
            diskBlocks[i].fileId = fileId;
            diskBlocks[i].nextBlock = -1;
            nextFreeBlock = i + 1;
            return i;
        }
    }
    return -1;
}
void createFileLinked(int fileId, char *fileName, int fileSize) {
    if (fileCount >= MAX_FILES) {
        printf("File table is full!\n");
        return;
    }
    int blocksNeeded = (fileSize + 511) / 512;
    printf("File '%s' created with linked allocation\n", fileName);
    printf("  - Blocks Required: %d\n", blocksNeeded);
    fileTable[fileCount].fileId = fileId;
    strcpy(fileTable[fileCount].fileName, fileName);
    fileTable[fileCount].fileSize = fileSize;
    fileTable[fileCount].blockCount = 0;
    int firstBlock = -1;
    int prevBlock = -1;
    for (int i = 0; i < blocksNeeded; i++) {
        int currentBlock = allocateBlock(fileId);
        if (currentBlock == -1) {
            printf("Warning: Could not allocate block %d\n", i);
            break;
        }
        if (firstBlock == -1) {
            firstBlock = currentBlock;
        } else {
            diskBlocks[prevBlock].nextBlock = currentBlock;
        }
        prevBlock = currentBlock;
        fileTable[fileCount].blockCount++;
        printf("  Block %d allocated (next: -1)\n", currentBlock);
    }
    fileTable[fileCount].firstBlock = firstBlock;
    fileTable[fileCount].lastBlock = prevBlock;
    printf("  First Block: %d, Last Block: %d\n\n", firstBlock, prevBlock);
    fileCount++;
}
void displayLinkedAllocationTable() {
    printf("\n===== File Allocation Table (Linked) =====\n");
    printf("%-10s %-20s %-10s %-12s %-12s %-15s\n",
           "File ID", "File Name", "Size", "First Block", "Last Block", "Block Count");
    printf("==============================================================\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%-10d %-20s %-10d %-12d %-12d %-15d\n",
               fileTable[i].fileId,
               fileTable[i].fileName,
               fileTable[i].fileSize,
               fileTable[i].firstBlock,
               fileTable[i].lastBlock,
               fileTable[i].blockCount);
    }
    printf("==============================================================\n\n");
}
void displayLinkedChain() {
    printf("===== Linked Block Chain Details =====\n\n");
    for (int i = 0; i < fileCount; i++) {
        printf("File '%s' (ID: %d)\n", fileTable[i].fileName, fileTable[i].fileId);
        printf("Block Chain: ");
        int currentBlock = fileTable[i].firstBlock;
        int blockNo = 1;
        while (currentBlock != -1) {
            printf("%d", currentBlock);
            if (diskBlocks[currentBlock].nextBlock != -1) {
                printf(" -> ");
            }
            currentBlock = diskBlocks[currentBlock].nextBlock;
            blockNo++;
        }
        printf("\n\n");
    }
}
void displayDiskAllocation() {
    printf("===== Disk Block Allocation =====\n");
    printf("Block Map: [ ");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (diskBlocks[i].isFree) {
            printf(". ");
        } else {
            printf("%d ", diskBlocks[i].fileId);
        }
    }
    printf("]\n");
    printf("(. = Free, Number = File ID)\n\n");
    printf("Block Links:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (!diskBlocks[i].isFree) {
            printf("  Block %d (File %d) -> Block %d\n",
                   i, diskBlocks[i].fileId,
                   (diskBlocks[i].nextBlock == -1) ? -1 : diskBlocks[i].nextBlock);
        }
    }
    printf("\n");
}
void displayDiskStatistics() {
    printf("===== Disk Statistics =====\n");
    int freeBlocks = 0;
    int usedBlocks = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (diskBlocks[i].isFree) {
            freeBlocks++;
        } else {
            usedBlocks++;
        }
    }
    printf("Total Blocks: %d\n", MAX_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", (usedBlocks * 100.0) / MAX_BLOCKS);
    printf("===========================\n\n");
}
void traverseFile(int fileId, int targetBlock) {
    for (int i = 0; i < fileCount; i++) {
        if (fileTable[i].fileId == fileId) {
            printf("File Access: '%s' (ID: %d)\n", fileTable[i].fileName, fileId);
            printf("Traversing to block %d:\n", targetBlock);
            int currentBlock = fileTable[i].firstBlock;
            int steps = 0;
            for (int j = 0; j < targetBlock && currentBlock != -1; j++) {
                printf("  Step %d: At block %d", j + 1, currentBlock);
                if (diskBlocks[currentBlock].nextBlock != -1) {
                    printf(" -> Follow link to block %d\n", diskBlocks[currentBlock].nextBlock);
                } else {
                    printf(" (end of file)\n");
                }
                currentBlock = diskBlocks[currentBlock].nextBlock;
                steps++;
            }
            if (currentBlock != -1) {
                printf("  Final: Reached block %d\n", currentBlock);
                printf("  Total traversal steps: %d\n\n", steps);
            } else {
                printf("  Error: Target block not found!\n\n");
            }
            return;
        }
    }
    printf("File not found!\n\n");
}
int main() {
    printf("======= Linked File Allocation Strategy =======\n");
    printf("(File blocks linked via pointers, scattered on disk)\n\n");
    initializeDisk();
    createFileLinked(1, "document.txt", 2048);
    createFileLinked(2, "image.jpg", 4096);
    createFileLinked(3, "video.mp4", 6144);
    displayLinkedAllocationTable();
    displayLinkedChain();
    displayDiskAllocation();
    displayDiskStatistics();
    printf("===== Block Access Examples =====\n");
    traverseFile(1, 2);
    traverseFile(2, 3);
    traverseFile(3, 4);
    printf("===== Characteristics of Linked Allocation =====\n");
    printf("1. Each block contains pointer to next block\n");
    printf("2. Directory stores first and last block pointers\n");
    printf("3. Data blocks can be scattered anywhere on disk\n");
    printf("4. No external fragmentation\n");
    printf("5. No wasted space for index blocks\n");
    printf("6. Sequential access is efficient\n");
    printf("7. Random access requires traversal from start\n");
    printf("8. Lost pointer means lost data\n");
    printf("=================================================\n");
    return 0;
}
