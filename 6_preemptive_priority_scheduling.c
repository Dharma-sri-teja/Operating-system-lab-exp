/*
* Program: Preemptive Priority Scheduling
* Description: Implements preemptive priority scheduling algorithm where
* a running process can be preempted if a higher priority process arrives
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
int remaining_time;
int completion_time;
int turnaround_time;
int waiting_time;
} Process;

void calculatePreemptivePriorityScheduling(Process processes[], int n) {
int current_time = 0;
int completed = 0;
int last_executed = -1;

printf("\n===== Preemptive Priority Scheduling =====\n\n");
printf("Time | Process Executed | Priority\n");
printf("-----|------------------|----------\n");

while (completed < n) {
int highest_priority = 999, highest_index = -1;

// Find process with highest priority (lowest number) that has arrived and not completed
for (int i = 0; i < n; i++) {
if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
if (processes[i].priority < highest_priority) {
highest_priority = processes[i].priority;
highest_index = i;
}
}
}

// If no process available, advance time to next arrival
if (highest_index == -1) {
int next_arrival = 999;
for (int i = 0; i < n; i++) {
if (processes[i].remaining_time > 0 && processes[i].arrival_time > current_time) {
if (processes[i].arrival_time < next_arrival) {
next_arrival = processes[i].arrival_time;
}
}
}
current_time = next_arrival;
continue;
}

// Execute the selected process for 1 unit time
current_time++;
processes[highest_index].remaining_time--;

// Print execution info (only when process changes)
if (last_executed != highest_index) {
printf(" %2d  |       P%d        |    %d\n",
current_time - 1,
processes[highest_index].process_id,
processes[highest_index].priority);
last_executed = highest_index;
}

// If process is completed
if (processes[highest_index].remaining_time == 0) {
processes[highest_index].completion_time = current_time;
processes[highest_index].turnaround_time = 
processes[highest_index].completion_time - processes[highest_index].arrival_time;
processes[highest_index].waiting_time = 
processes[highest_index].turnaround_time - processes[highest_index].burst_time;
completed++;
}
}
}

void displaySchedulingTable(Process processes[], int n) {
printf("\n===== Process Scheduling Details =====\n\n");
printf("Process | Priority | Burst Time | Completion Time | Turnaround Time | Waiting Time\n");
printf("--------|----------|------------|-----------------|-----------------|--------------\n");

for (int i = 0; i < n; i++) {
printf("   P%d   |    %d     |     %d      |        %d        |       %d        |      %d\n",
processes[i].process_id,
processes[i].priority,
processes[i].burst_time,
processes[i].completion_time,
processes[i].turnaround_time,
processes[i].waiting_time);
}
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

printf("========== Preemptive Priority Scheduling Algorithm ==========\n");
printf("Note: Lower priority number = Higher priority\n");
printf("      Process can be preempted if higher priority process arrives\n\n");

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

printf("\nProcess P%d:\n", i + 1);
printf("  Burst Time: ");
scanf("%d", &processes[i].burst_time);

printf("  Arrival Time: ");
scanf("%d", &processes[i].arrival_time);

printf("  Priority (1-10, 1=highest): ");
scanf("%d", &processes[i].priority);

// Initialize remaining time
processes[i].remaining_time = processes[i].burst_time;

if (processes[i].burst_time <= 0) {
printf("  Invalid burst time! Using 1 unit.\n");
processes[i].burst_time = 1;
processes[i].remaining_time = 1;
}
if (processes[i].arrival_time < 0) {
processes[i].arrival_time = 0;
}
if (processes[i].priority < 1 || processes[i].priority > 10) {
processes[i].priority = 5;
}
}

// Calculate preemptive priority scheduling
calculatePreemptivePriorityScheduling(processes, n);

// Display scheduling table
displaySchedulingTable(processes, n);

// Calculate and display averages
calculateAverages(processes, n);

return 0;
}

/*
* Compilation and Execution:
* gcc -o preemptive_priority_scheduling 6_preemptive_priority_scheduling.c
* ./preemptive_priority_scheduling
* 
* Example Input:
* Processes: 3
* P1: Burst=8, Arrival=0, Priority=3
* P2: Burst=4, Arrival=1, Priority=1
* P3: Burst=2, Arrival=2, Priority=2
* 
* Key Concepts:
* 1. Preemptive: A running process can be interrupted if higher priority process arrives
* 2. Process with highest priority (lowest number) is selected next
* 3. Executed for 1 unit of time at a time to check for preemption
* 
* Advantages:
* - More responsive to high priority processes
* - High priority processes don't wait long
* - Better for real-time systems
* 
* Disadvantages:
* - Higher overhead due to context switching
* - Lower priority processes may suffer starvation
* - Complex to implement
*/

