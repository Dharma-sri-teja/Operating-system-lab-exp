/*
* Program: Shortest Job First (SJF) - Preemptive (SRTF - Shortest Remaining Time First)
* Description: Implements preemptive SJF scheduling algorithm where
* process with shortest remaining execution time is selected to run
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

typedef struct {
int process_id;
int burst_time;
int arrival_time;
int remaining_time;
int completion_time;
int turnaround_time;
int waiting_time;
} Process;

void calculatePreemptiveSJF(Process processes[], int n) {
int current_time = 0;
int completed = 0;
int last_executed = -1;

printf("\n===== Preemptive SJF (SRTF) Scheduling =====\n\n");
printf("Time | Process Executed | Remaining Time\n");
printf("-----|------------------|----------------\n");

while (completed < n) {
int min_remaining = 999, min_index = -1;

// Find process with minimum remaining time that has arrived and not completed
for (int i = 0; i < n; i++) {
if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
if (processes[i].remaining_time < min_remaining) {
min_remaining = processes[i].remaining_time;
min_index = i;
}
}
}

// If no process available, advance time to next arrival
if (min_index == -1) {
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
processes[min_index].remaining_time--;

// Print execution info (only when process changes)
if (last_executed != min_index) {
printf(" %2d  |       P%d        |       %d\n",
current_time - 1,
processes[min_index].process_id,
processes[min_index].remaining_time);
last_executed = min_index;
}

// If process is completed
if (processes[min_index].remaining_time == 0) {
processes[min_index].completion_time = current_time;
processes[min_index].turnaround_time = 
processes[min_index].completion_time - processes[min_index].arrival_time;
processes[min_index].waiting_time = 
processes[min_index].turnaround_time - processes[min_index].burst_time;
completed++;
}
}
}

void displaySchedulingTable(Process processes[], int n) {
printf("\n===== Process Scheduling Details =====\n\n");
printf("Process | Burst Time | Completion Time | Turnaround Time | Waiting Time\n");
printf("--------|------------|-----------------|-----------------|--------------\n");

for (int i = 0; i < n; i++) {
printf("   P%d   |     %d      |        %d        |       %d        |      %d\n",
processes[i].process_id,
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

printf("========== Preemptive SJF (SRTF) CPU Scheduling Algorithm ==========\n");
printf("Note: Process with shortest remaining time is executed next\n");
printf("      Process can be preempted if shorter job arrives\n\n");

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
}

// Calculate preemptive SJF scheduling
calculatePreemptiveSJF(processes, n);

// Display scheduling table
displaySchedulingTable(processes, n);

// Calculate and display averages
calculateAverages(processes, n);

return 0;
}

/*
* Compilation and Execution:
* gcc -o preemptive_sjf 7_preemptive_sjf.c
* ./preemptive_sjf
* 
* Example Input:
* Processes: 3
* P1: Burst=8, Arrival=0
* P2: Burst=4, Arrival=1
* P3: Burst=2, Arrival=2
* 
* Key Concepts:
* 1. Preemptive: A running process can be interrupted if shorter job arrives
* 2. SRTF = Shortest Remaining Time First
* 3. Process with least remaining time is selected
* 4. Executed for 1 unit of time at a time to check for preemption
* 
* Advantages:
* - Optimal average waiting time for preemptive algorithms
* - Short jobs don't wait long
* 
* Disadvantages:
* - High overhead due to frequent context switching
* - Difficult to implement in practice
* - Burst time prediction still needed
* - Long processes suffer starvation
*/

