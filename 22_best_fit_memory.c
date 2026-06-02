#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 1000
#define MAX_BLOCKS 10

typedef struct {
    int block_id;
    int start_address;
    int size;
    int is_allocated;
    char process_name[50];
} MemoryBlock;

MemoryBlock memory[MAX_BLOCKS];
int block_count = 0;

void initialize_memory() {
    printf("Initializing memory with total size: %d bytes\n", MAX_MEMORY);
    
    memory[0].block_id = 0;
    memory[0].start_address = 0;
    memory[0].size = MAX_MEMORY;
    memory[0].is_allocated = 0;
    strcpy(memory[0].process_name, "Free");
    
    block_count = 1;
}

void display_memory() {
    printf("\n========== Memory Layout ==========\n");
    printf("Block# | Start | Size  | Status      | Process\n");
    printf("=============================================\n");
    
    for (int i = 0; i < block_count; i++) {
        printf("%3d    | %5d | %5d | %-11s | %s\n",
               memory[i].block_id,
               memory[i].start_address,
               memory[i].size,
               memory[i].is_allocated ? "Allocated" : "Free",
               memory[i].process_name);
    }
    printf("=============================================\n");
}

void get_memory_stats() {
    int total_free = 0;
    int total_allocated = 0;
    int largest_free = 0;
    
    for (int i = 0; i < block_count; i++) {
        if (memory[i].is_allocated) {
            total_allocated += memory[i].size;
        } else {
            total_free += memory[i].size;
            if (memory[i].size > largest_free) {
                largest_free = memory[i].size;
            }
        }
    }
    
    printf("\n--- Memory Statistics ---\n");
    printf("Total Memory: %d bytes\n", MAX_MEMORY);
    printf("Allocated: %d bytes\n", total_allocated);
    printf("Free: %d bytes\n", total_free);
    printf("Largest Free Block: %d bytes\n", largest_free);
}

int best_fit_allocation(char *process_name, int size) {
    int best_idx = -1;
    int best_size = MAX_MEMORY + 1;
    
    printf("\nAllocating %d bytes for %s using Best Fit algorithm:\n", size, process_name);
    
    // Find the best fit (smallest free block that fits)
    for (int i = 0; i < block_count; i++) {
        if (!memory[i].is_allocated && memory[i].size >= size) {
            if (memory[i].size < best_size) {
                best_size = memory[i].size;
                best_idx = i;
            }
        }
    }
    
    if (best_idx == -1) {
        printf("ERROR: Cannot allocate %d bytes - No suitable block found!\n", size);
        return 0;
    }
    
    printf("Best fit block found: Block %d (size: %d)\n", best_idx, memory[best_idx].size);
    
    // Split the block if necessary
    if (memory[best_idx].size > size) {
        // Create new block for remaining free space
        if (block_count >= MAX_BLOCKS) {
            printf("ERROR: Maximum blocks reached!\n");
            return 0;
        }
        
        // Shift blocks
        for (int i = block_count; i > best_idx + 1; i--) {
            memory[i] = memory[i - 1];
        }
        
        // Create free block with remaining space
        memory[best_idx + 1].block_id = best_idx + 1;
        memory[best_idx + 1].start_address = memory[best_idx].start_address + size;
        memory[best_idx + 1].size = memory[best_idx].size - size;
        memory[best_idx + 1].is_allocated = 0;
        strcpy(memory[best_idx + 1].process_name, "Free");
        
        block_count++;
    }
    
    // Allocate the block
    memory[best_idx].size = size;
    memory[best_idx].is_allocated = 1;
    strcpy(memory[best_idx].process_name, process_name);
    
    printf("Successfully allocated %d bytes to %s at address %d\n",
           size, process_name, memory[best_idx].start_address);
    
    return 1;
}

void deallocate_memory(char *process_name) {
    printf("\nDeallocating memory for %s:\n", process_name);
    
    for (int i = 0; i < block_count; i++) {
        if (memory[i].is_allocated && strcmp(memory[i].process_name, process_name) == 0) {
            printf("Deallocated %d bytes from address %d\n",
                   memory[i].size, memory[i].start_address);
            
            memory[i].is_allocated = 0;
            strcpy(memory[i].process_name, "Free");
            
            // Merge with adjacent free blocks
            // Merge with previous block
            if (i > 0 && !memory[i-1].is_allocated) {
                memory[i-1].size += memory[i].size;
                for (int j = i; j < block_count - 1; j++) {
                    memory[j] = memory[j + 1];
                }
                block_count--;
                i--;
            }
            
            // Merge with next block
            if (i < block_count - 1 && !memory[i+1].is_allocated) {
                memory[i].size += memory[i+1].size;
                for (int j = i + 1; j < block_count - 1; j++) {
                    memory[j] = memory[j + 1];
                }
                block_count--;
            }
            
            return;
        }
    }
    
    printf("ERROR: Process '%s' not found!\n", process_name);
}

int main() {
    printf("Best Fit Memory Allocation Algorithm\n");
    printf("===================================\n");
    printf("Best Fit: Allocate from the SMALLEST available block that fits\n\n");
    
    initialize_memory();
    
    display_memory();
    
    // Allocate memory to various processes
    best_fit_allocation("Process_A", 150);
    display_memory();
    get_memory_stats();
    
    best_fit_allocation("Process_B", 200);
    display_memory();
    get_memory_stats();
    
    best_fit_allocation("Process_C", 100);
    display_memory();
    get_memory_stats();
    
    best_fit_allocation("Process_D", 250);
    display_memory();
    get_memory_stats();
    
    printf("\n========== Deallocating Process_B ==========\n");
    deallocate_memory("Process_B");
    display_memory();
    get_memory_stats();
    
    printf("\n========== Allocating Process_E (150 bytes) ==========\n");
    best_fit_allocation("Process_E", 150);
    display_memory();
    get_memory_stats();
    
    printf("\n========== Analysis ==========\n");
    printf("Best Fit advantages:\n");
    printf("- Minimizes wasted space\n");
    printf("- Good memory utilization\n");
    printf("Best Fit disadvantages:\n");
    printf("- Slower allocation (searches all blocks)\n");
    printf("- Can lead to fragmentation with many small blocks\n");
    
    return 0;
}