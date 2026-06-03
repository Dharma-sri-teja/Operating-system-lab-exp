#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BLOCKS 100
#define MAX_FILES 10
#define MAX_FILENAME 50
typedef struct {
    int fileId;
    char fileName[MAX_FILENAME];
    int fileSize;
    int startBlock;
    int blockCount;
} File;
typedef struct {
    int blockNo;
    int isFree;
    int fileId;
} Block;
File fileTable[MAX_FILES];
Block diskBlocks[MAX_BLOCKS];
int fileCount = 0;
void initializeDisk() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        diskBlocks[i].blockNo = i;
        diskBlocks[i].isFree = 1;
        diskBlocks[i].fileId = -1;
    }
}
void createFileSequential(int fileId, char *fileName, int fileSize) {
    if (fileCount >= MAX_FILES) {
        printf("File table is full!\n");
        return;
    }
    int blocksNeeded = (fileSize + 511) / 512; 
    int blocksAllocated = 0;
    int startBlock = -1;
    int currentBlock = -1;
    for (int i = 0; i < MAX_BLOCKS && blocksAllocated < blocksNeeded; i++) {
        if (diskBlocks[i].isFree) {
            if (startBlock == -1) {
                startBlock = i;
                currentBlock = i;
            } else if (diskBlocks[i - 1].fileId == fileId) {
                currentBlock = i;
            } else {
                if (blocksAllocated < blocksNeeded) {
                    startBlock = i;
                    currentBlock = i;
                    blocksAllocated = 0;
                }
            }
            diskBlocks[i].isFree = 0;
            diskBlocks[i].fileId = fileId;
            blocksAllocated++;
        } else if (blocksAllocated < blocksNeeded) {
            blocksAllocated = 0;
            startBlock = -1;
        }
    }
    if (blocksAllocated < blocksNeeded) {
        printf("Not enough contiguous space! Allocated only %d blocks (needed %d)\n", 
               blocksAllocated, blocksNeeded);
        for (int i = 0; i < MAX_BLOCKS; i++) {
            if (diskBlocks[i].fileId == fileId) {
                diskBlocks[i].isFree = 1;
                diskBlocks[i].fileId = -1;
            }
        }
        return;
    }
    fileTable[fileCount].fileId = fileId;
    strcpy(fileTable[fileCount].fileName, fileName);
    fileTable[fileCount].fileSize = fileSize;
    fileTable[fileCount].startBlock = startBlock;
    fileTable[fileCount].blockCount = blocksAllocated;
    printf("File '%s' created sequentially\n", fileName);
    printf("  - Start Block: %d\n", startBlock);
    printf("  - Blocks Allocated: %d\n", blocksAllocated);
    printf("  - Block Range: %d-%d\n\n", startBlock, startBlock + blocksAllocated - 1);
    fileCount++;
}
void displayFileAllocationTable() {
    printf("\n===== File Allocation Table (Sequential) =====\n");
    printf("%-10s %-20s %-10s %-12s %-15s\n", 
           "File ID", "File Name", "Size", "Start Block", "Block Count");
    printf("=====================================================\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%-10d %-20s %-10d %-12d %-15d\n",
               fileTable[i].fileId,
               fileTable[i].fileName,
               fileTable[i].fileSize,
               fileTable[i].startBlock,
               fileTable[i].blockCount);
    }
    printf("=====================================================\n\n");
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
}
void displayDiskStatistics() {
    printf("===== Disk Statistics =====\n");
    int freeBlocks = 0;
    int usedBlocks = 0;
    int fragmentation = 0;
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
void accessFile(int fileId, int recordNo) {
    for (int i = 0; i < fileCount; i++) {
        if (fileTable[i].fileId == fileId) {
            printf("Accessing File '%s' (ID: %d)\n", fileTable[i].fileName, fileId);
            printf("Record %d access requires reading from block %d\n",
                   recordNo, fileTable[i].startBlock + recordNo);
            printf("Access sequence: Read blocks %d to %d\n",
                   fileTable[i].startBlock, fileTable[i].startBlock + recordNo);
            printf("(All records must be read sequentially from the beginning)\n\n");
            return;
        }
    }
    printf("File not found!\n\n");
}
int main() {
    printf("======= Sequential File Allocation Strategy =======\n");
    printf("(All file records stored one after another physically)\n\n");
    initializeDisk();
    createFileSequential(1, "document.txt", 2048);
    createFileSequential(2, "image.jpg", 4096);
    createFileSequential(3, "audio.mp3", 3072);
    displayFileAllocationTable();
    displayDiskAllocation();
    displayDiskStatistics();
    printf("===== File Access Examples =====\n");
    accessFile(1, 2);
    accessFile(2, 5);
    accessFile(3, 1);
    printf("===== Characteristics of Sequential Allocation =====\n");
    printf("1. Records stored physically contiguous\n");
    printf("2. No gaps between records\n");
    printf("3. Access requires reading all previous records\n");
    printf("4. Efficient for sequential access patterns\n");
    printf("5. External fragmentation not an issue\n");
    printf("6. Random access is slow\n");
    printf("======================================================\n");
    return 0;
}
