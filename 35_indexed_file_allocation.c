#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BLOCKS 100
#define MAX_FILES 10
#define MAX_INDEX_ENTRIES 50
#define MAX_FILENAME 50
typedef struct {
    int blockNo;
} IndexEntry;
typedef struct {
    int fileId;
    char fileName[MAX_FILENAME];
    int fileSize;
    int indexBlockNo;
    int blockCount;
    IndexEntry indexBlock[MAX_INDEX_ENTRIES];
} File;
typedef struct {
    int blockNo;
    int isFree;
    int fileId;
    int type; 
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
        diskBlocks[i].type = 0;
    }
    nextFreeBlock = 0;
}
int allocateBlock(int fileId, int type) {
    for (int i = nextFreeBlock; i < MAX_BLOCKS; i++) {
        if (diskBlocks[i].isFree) {
            diskBlocks[i].isFree = 0;
            diskBlocks[i].fileId = fileId;
            diskBlocks[i].type = type;
            nextFreeBlock = i + 1;
            return i;
        }
    }
    return -1;
}
void createFileIndexed(int fileId, char *fileName, int fileSize) {
    if (fileCount >= MAX_FILES) {
        printf("File table is full!\n");
        return;
    }
    int blocksNeeded = (fileSize + 511) / 512;
    int indexBlockNo = allocateBlock(fileId, 1);
    if (indexBlockNo == -1) {
        printf("Failed to allocate index block!\n");
        return;
    }
    printf("File '%s' created with indexed allocation\n", fileName);
    printf("  - Index Block: %d\n", indexBlockNo);
    printf("  - Blocks Required: %d\n\n", blocksNeeded);
    fileTable[fileCount].fileId = fileId;
    strcpy(fileTable[fileCount].fileName, fileName);
    fileTable[fileCount].fileSize = fileSize;
    fileTable[fileCount].indexBlockNo = indexBlockNo;
    fileTable[fileCount].blockCount = 0;
    for (int i = 0; i < blocksNeeded; i++) {
        int dataBlock = allocateBlock(fileId, 0);
        if (dataBlock == -1) {
            printf("Warning: Could not allocate block %d\n", i);
            break;
        }
        fileTable[fileCount].indexBlock[i].blockNo = dataBlock;
        fileTable[fileCount].blockCount++;
        printf("  Index[%d] -> Block %d\n", i, dataBlock);
    }
    fileCount++;
    printf("\n");
}
void displayIndexAllocationTable() {
    printf("\n===== File Allocation Table (Indexed) =====\n");
    printf("%-10s %-20s %-10s %-12s %-15s\n",
           "File ID", "File Name", "Size", "Index Block", "Data Blocks");
    printf("=======================================================\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%-10d %-20s %-10d %-12d %-15d\n",
               fileTable[i].fileId,
               fileTable[i].fileName,
               fileTable[i].fileSize,
               fileTable[i].indexBlockNo,
               fileTable[i].blockCount);
    }
    printf("=======================================================\n\n");
}
void displayIndexDetails() {
    printf("===== Index Block Details =====\n\n");
    for (int i = 0; i < fileCount; i++) {
        printf("File '%s' (ID: %d)\n", fileTable[i].fileName, fileTable[i].fileId);
        printf("Index Block %d contains:\n", fileTable[i].indexBlockNo);
        for (int j = 0; j < fileTable[i].blockCount; j++) {
            printf("  Index[%d] -> Data Block %d\n", j, fileTable[i].indexBlock[j].blockNo);
        }
        printf("\n");
    }
}
void displayDiskAllocation() {
    printf("===== Disk Block Allocation =====\n");
    printf("Block Map: [ ");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (diskBlocks[i].isFree) {
            printf(". ");
        } else if (diskBlocks[i].type == 1) {
            printf("I ");  
        } else {
            printf("%d ", diskBlocks[i].fileId);  
        }
    }
    printf("]\n");
    printf("(. = Free, I = Index Block, Number = File ID Data Block)\n\n");
}
void displayDiskStatistics() {
    printf("===== Disk Statistics =====\n");
    int freeBlocks = 0;
    int indexBlocks = 0;
    int dataBlocks = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (diskBlocks[i].isFree) {
            freeBlocks++;
        } else if (diskBlocks[i].type == 1) {
            indexBlocks++;
        } else {
            dataBlocks++;
        }
    }
    printf("Total Blocks: %d\n", MAX_BLOCKS);
    printf("Index Blocks: %d\n", indexBlocks);
    printf("Data Blocks: %d\n", dataBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", ((indexBlocks + dataBlocks) * 100.0) / MAX_BLOCKS);
    printf("===========================\n\n");
}
void accessBlock(int fileId, int blockIndex) {
    for (int i = 0; i < fileCount; i++) {
        if (fileTable[i].fileId == fileId) {
            if (blockIndex < fileTable[i].blockCount) {
                printf("File Access: '%s' (ID: %d)\n", fileTable[i].fileName, fileId);
                printf("Accessing block %d:\n", blockIndex);
                printf("  1. Read index block %d\n", fileTable[i].indexBlockNo);
                printf("  2. Read index entry %d\n", blockIndex);
                printf("  3. Access data block %d\n\n", fileTable[i].indexBlock[blockIndex].blockNo);
            } else {
                printf("Block index %d out of range!\n\n", blockIndex);
            }
            return;
        }
    }
    printf("File not found!\n\n");
}
int main() {
    printf("======= Indexed File Allocation Strategy =======\n");
    printf("(All file pointers stored in an index block)\n\n");
    initializeDisk();
    createFileIndexed(1, "document.txt", 2048);
    createFileIndexed(2, "image.jpg", 4096);
    createFileIndexed(3, "video.mp4", 8192);
    displayIndexAllocationTable();
    displayIndexDetails();
    displayDiskAllocation();
    displayDiskStatistics();
    printf("===== Block Access Examples =====\n");
    accessBlock(1, 0);
    accessBlock(1, 2);
    accessBlock(2, 3);
    accessBlock(3, 5);
    printf("===== Characteristics of Indexed Allocation =====\n");
    printf("1. All file pointers collected in index block\n");
    printf("2. Supports random access efficiently\n");
    printf("3. Data blocks can be scattered anywhere on disk\n");
    printf("4. No external fragmentation\n");
    printf("5. Extra overhead for index blocks\n");
    printf("6. Requires index block allocation upfront\n");
    printf("7. Index block size limits file size\n");
    printf("=================================================\n");
    return 0;
}
