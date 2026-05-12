/*
* Program: Shortest Job First (SJF) CPU Scheduling - Non-Preemptive
* Description: Implements non-preemptive SJF scheduling algorithm where
* the process with smallest execution time is selected next
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

typedef struct {
int process_id;
int burst_time;
int arrival_time;
int completion_time;
int turnaround_time;
int waiting_time;
int executed;  // Flag to mark if process has been executed
} Process;

void calculateSJF(Process processes[], int n) {
int current_time = 0;
int executed_count = 0;

printf("\n===== Non-Preemptive SJF Scheduling =====\n\n");
printf("Process | Burst Time | Completion Time | Turnaround Time | Waiting Time\n");
printf("--------|------------|-----------------|-----------------|--------------\n");

while (executed_count < n) {
int min_burst = 999, min_index = -1;

// Find process with minimum burst time that has arrived
for (int i = 0; i < n; i++) {
if (!processes[i].executed && processes[i].arrival_time <= current_time) {
if (processes[i].burst_time < min_burst) {
min_burst = processes[i].burst_time;
min_index = i;
}
}
}

// If no process found, find process with earliest arrival time
if (min_index == -1) {
for (int i = 0; i < n; i++) {
if (!processes[i].executed) {
min_index = i;
break;
}
}
current_time = processes[min_index].arrival_time;
}

// Execute the selected process
current_time += processes[min_index].burst_time;
processes[min_index].completion_time = current_time;
processes[min_index].turnaround_time = processes[min_index].completion_time - processes[min_index].arrival_time;
processes[min_index].waiting_time = processes[min_index].turnaround_time - processes[min_index].burst_time;
processes[min_index].executed = 1;
executed_count++;

printf("   P%d   |     %d      |        %d        |       %d        |      %d\n",
processes[min_index].process_id,
processes[min_index].burst_time,
processes[min_index].completion_time,
processes[min_index].turnaround_time,
processes[min_index].waiting_time);
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

printf("========== Non-Preemptive SJF CPU Scheduling Algorithm ==========\n\n");

// Input number of processes
printf("Enter number of processes (max %d): ", MAX_PROCESSES);
scanf("%d", &n);

if (n <= 0 || n > MAX_PROCESSES) {
printf("Invalid number of processes!\n");
return 1;
}

// Input process details
printf("\nEnter process details (PID, burst time, arrival time):\n");
for (int i = 0; i < n; i++) {
processes[i].process_id = i + 1;
processes[i].executed = 0;

printf("Process P%d - Burst Time: ", i + 1);
scanf("%d", &processes[i].burst_time);

printf("Process P%d - Arrival Time: ", i + 1);
scanf("%d", &processes[i].arrival_time);

if (processes[i].burst_time <= 0) {
printf("Invalid burst time! Using 1 unit.\n");
processes[i].burst_time = 1;
}
if (processes[i].arrival_time < 0) {
processes[i].arrival_time = 0;
}
}

// Calculate SJF scheduling
calculateSJF(processes, n);

// Display Gantt chart
displayGanttChart(processes, n);

// Calculate and display averages
calculateAverages(processes, n);

return 0;
}

/*
* Compilation and Execution:
* gcc -o sjf_scheduling 4_sjf_scheduling.c
* ./sjf_scheduling
* 
* Example Input:
* Processes: 3
* P1: Burst=8, Arrival=0
* P2: Burst=4, Arrival=0
* P3: Burst=2, Arrival=0
* Expected execution order: P3 (2), P2 (4), P1 (8)
* 
* Key Concepts:
* 1. Non-preemptive: Once a process starts, it runs to completion
* 2. The process with shortest burst time is selected next
* 3. Optimal for average waiting time
* 
* Advantages:
* - Minimizes average waiting time
* - Minimizes average turnaround time
* 
* Disadvantages:
* - Difficult to estimate burst times in advance
* - Long processes may suffer starvation (convoy effect)
* - Not suitable for time-sharing systems
*/

