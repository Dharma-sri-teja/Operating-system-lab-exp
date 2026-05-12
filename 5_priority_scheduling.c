/*
* Program: Non-Preemptive Priority Scheduling
* Description: Implements non-preemptive priority scheduling algorithm where
* the process with highest priority (lower number = higher priority) is selected next
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

typedef struct {
int process_id;
int burst_time;
int arrival_time;
int priority;
int completion_time;
int turnaround_time;
int waiting_time;
int executed;  // Flag to mark if process has been executed
} Process;

void calculatePriorityScheduling(Process processes[], int n) {
int current_time = 0;
int executed_count = 0;

printf("\n===== Non-Preemptive Priority Scheduling =====\n\n");
printf("Process | Priority | Burst Time | Completion Time | Turnaround Time | Waiting Time\n");
printf("--------|----------|------------|-----------------|-----------------|--------------\n");

while (executed_count < n) {
int max_priority = 999, max_index = -1;

// Find process with highest priority (lowest priority number) that has arrived
for (int i = 0; i < n; i++) {
if (!processes[i].executed && processes[i].arrival_time <= current_time) {
if (processes[i].priority < max_priority) {
max_priority = processes[i].priority;
max_index = i;
}
}
}

// If no process found, find process with earliest arrival time
if (max_index == -1) {
for (int i = 0; i < n; i++) {
if (!processes[i].executed) {
max_index = i;
break;
}
}
current_time = processes[max_index].arrival_time;
}

// Execute the selected process
current_time += processes[max_index].burst_time;
processes[max_index].completion_time = current_time;
processes[max_index].turnaround_time = processes[max_index].completion_time - processes[max_index].arrival_time;
processes[max_index].waiting_time = processes[max_index].turnaround_time - processes[max_index].burst_time;
processes[max_index].executed = 1;
executed_count++;

printf("   P%d   |    %d     |     %d      |        %d        |       %d        |      %d\n",
processes[max_index].process_id,
processes[max_index].priority,
processes[max_index].burst_time,
processes[max_index].completion_time,
processes[max_index].turnaround_time,
processes[max_index].waiting_time);
}
}

void displayGanttChart(Process processes[], int n) {
printf("\n===== Gantt Chart =====\n");

// Create a temporary array sorted by execution order
Process temp[MAX_PROCESSES];
memcpy(temp, processes, sizeof(Process) * n);

// Sort by completion time to show execution order
for (int i = 0; i < n - 1; i++) {
for (int j = i + 1; j < n; j++) {
if (temp[i].completion_time > temp[j].completion_time) {
Process swap = temp[i];
temp[i] = temp[j];
temp[j] = swap;
}
}
}

printf("Timeline: 0 ");
for (int i = 0; i < n; i++) {
printf("%d ", temp[i].completion_time);
}
printf("\n");

printf("Process:   ");
for (int i = 0; i < n; i++) {
printf("|  P%d  ", temp[i].process_id);
}
printf("|\n");
}

void calculateAverages(Process processes[], int n) {
float avg_turnaround = 0, avg_waiting = 0;

for (int i = 0; i < n; i++) {
avg_turnaround += processes[i].turnaround_time;
avg_waiting += processes[i].waiting_time;
}

avg_turnaround /= n;
avg_waiting /= n;

printf("\n===== Statistics =====\n");
printf("Average Turnaround Time: %.2f\n", avg_turnaround);
printf("Average Waiting Time: %.2f\n", avg_waiting);
printf("=======================\n");
}

int main() {
int n;
Process processes[MAX_PROCESSES];

printf("========== Non-Preemptive Priority Scheduling Algorithm ==========\n");
printf("Note: Lower priority number = Higher priority\n\n");

// Input number of processes
printf("Enter number of processes (max %d): ", MAX_PROCESSES);
scanf("%d", &n);

if (n <= 0 || n > MAX_PROCESSES) {
printf("Invalid number of processes!\n");
return 1;
}

// Input process details
printf("\nEnter process details:\n");
for (int i = 0; i < n; i++) {
processes[i].process_id = i + 1;
processes[i].executed = 0;

printf("\nProcess P%d:\n", i + 1);
printf("  Burst Time: ");
scanf("%d", &processes[i].burst_time);

printf("  Arrival Time: ");
scanf("%d", &processes[i].arrival_time);

printf("  Priority (1-10, 1=highest): ");
scanf("%d", &processes[i].priority);

if (processes[i].burst_time <= 0) {
printf("  Invalid burst time! Using 1 unit.\n");
processes[i].burst_time = 1;
}
if (processes[i].arrival_time < 0) {
processes[i].arrival_time = 0;
}
if (processes[i].priority < 1 || processes[i].priority > 10) {
processes[i].priority = 5;
}
}

// Calculate priority scheduling
calculatePriorityScheduling(processes, n);

// Display Gantt chart
displayGanttChart(processes, n);

// Calculate and display averages
calculateAverages(processes, n);

return 0;
}

/*
* Compilation and Execution:
* gcc -o priority_scheduling 5_priority_scheduling.c
* ./priority_scheduling
* 
* Example Input:
* Processes: 3
* P1: Burst=8, Arrival=0, Priority=3
* P2: Burst=4, Arrival=0, Priority=1
* P3: Burst=2, Arrival=0, Priority=2
* Expected execution order: P2 (priority 1), P3 (priority 2), P1 (priority 3)
* 
* Key Concepts:
* 1. Non-preemptive: Once a process starts, it runs to completion
* 2. Process with highest priority (lowest number) is selected next
* 3. Priority can be assigned internally or externally
* 
* Advantages:
* - Flexible scheduling based on process requirements
* - Can prioritize important processes
* 
* Disadvantages:
* - Low priority processes may suffer starvation
* - Complex to implement fairly
* - Requires priority assignment mechanism
*/

