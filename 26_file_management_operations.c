#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILES 10
#define MAX_BLOCKS 100
typedef struct {
    int fid, size, start, cnt;
} File;
typedef struct {
    int free, fid;
} Block;
File ftable[MAX_FILES];
Block disk[MAX_BLOCKS];
int fcnt = 0;
void init() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        disk[i].free = 1;
        disk[i].fid = -1;
    }
}
void create(int fid, char *fname, int size) {
    if (fcnt >= MAX_FILES) {
        printf("File table full\n");
        return;
    }
    int need = (size + 511) / 512;
    int alloc = 0, start = -1;
    for (int i = 0; i < MAX_BLOCKS && alloc < need; i++) {
        if (disk[i].free) {
            if (start == -1) start = i;
            disk[i].free = 0;
            disk[i].fid = fid;
            alloc++;
        }
    }
    if (alloc < need) {
        printf("Not enough space\n");
        return;
    }
    ftable[fcnt].fid = fid;
    ftable[fcnt].size = size;
    ftable[fcnt].start = start;
    ftable[fcnt].cnt = alloc;
    printf("File %d created: blocks %d-%d\n", fid, start, start + alloc - 1);
    fcnt++;
}
void del(int fid) {
    int idx = -1;
    for (int i = 0; i < fcnt; i++) {
        if (ftable[i].fid == fid) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("File not found\n");
        return;
    }
    for (int i = ftable[idx].start; i < ftable[idx].start + ftable[idx].cnt; i++) {
        disk[i].free = 1;
        disk[i].fid = -1;
    }
    for (int i = idx; i < fcnt - 1; i++) {
        ftable[i] = ftable[i + 1];
    }
    fcnt--;
    printf("File deleted\n");
}
void display() {
    printf("\nFiles:\n");
    for (int i = 0; i < fcnt; i++) {
        printf("F%d: size=%d blocks=%d-%d\n", ftable[i].fid, ftable[i].size, 
               ftable[i].start, ftable[i].start + ftable[i].cnt - 1);
    }
}
int main() {
    init();
    create(1, "doc.txt", 2048);
    create(2, "img.jpg", 4096);
    create(3, "vid.mp4", 8192);
    display();
    del(2);
    display();
    return 0;
    for (int i = 0; i < fileCount; i++) {
        printf("%-10d %-20s %-15d %-15d %-15d\n",
               fileTable[i].fileId,
               fileTable[i].fileName,
               fileTable[i].fileSize,
               fileTable[i].startBlock,
               fileTable[i].blockCount);
    }
    printf("================================\n\n");
}
void displayDiskUsage() {
    printf("\n========== Disk Usage ==========\n");
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
    printf("================================\n\n");
}
int main() {
    initializeDisk();
    printf("======== File Management System ========\n\n");
    createFile(1, "document.txt", 2048);
    createFile(2, "image.jpg", 5120);
    createFile(3, "video.mp4", 10240);
    displayFiles();
    displayDiskUsage();
    deleteFile(2);
    printf("After deletion:\n");
    displayFiles();
    displayDiskUsage();
    return 0;
}
