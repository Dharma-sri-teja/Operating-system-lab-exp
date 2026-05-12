/*
* Program: Round Robin (RR) CPU Scheduling
* Description: Implements Round Robin scheduling algorithm where
* each process gets a fixed time quantum (time slice) to execute
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_QUEUE 100

typedef struct {
int process_id;
int burst_time;
int arrival_time;
int remaining_time;
int completion_time;
int turnaround_time;
int waiting_time;
} Process;

typedef struct {
int process_id;
int time;
} QueueEntry;

void calculateRoundRobin(Process processes[], int n, int time_quantum) {
QueueEntry queue[MAX_QUEUE];
int queue_front = 0, queue_rear = 0;
int current_time = 0;
int completed = 0;

printf("\n===== Round Robin (RR) Scheduling (Time Quantum = %d) =====\n\n", time_quantum);
printf("Time | Process | Remaining Time | Action\n");
printf("-----|---------|----------------|-----------\n");

// Add all processes with arrival time 0 to queue
for (int i = 0; i < n; i++) {
if (processes[i].arrival_time == 0) {
queue[queue_rear].process_id = processes[i].process_id;
queue[queue_rear].time = i;
queue_rear++;
}
}

while (queue_front < queue_rear || completed < n) {
// If queue is empty, advance time to next arrival
if (queue_front >= queue_rear) {
int next_arrival = 999;
for (int i = 0; i < n; i++) {
if (processes[i].remaining_time > 0 && 
processes[i].arrival_time > current_time &&
processes[i].arrival_time < next_arrival) {
next_arrival = processes[i].arrival_time;
}
}
current_time = next_arrival;

// Add all processes arriving at this time
for (int i = 0; i < n; i++) {
if (processes[i].arrival_time == current_time && processes[i].remaining_time > 0) {
queue[queue_rear].process_id = processes[i].process_id;
queue[queue_rear].time = i;
queue_rear++;
}
}
continue;
}

// Get process from front of queue
int proc_index = queue[queue_front].time;
int process_id = queue[queue_front].process_id;
queue_front++;

// Execute for time quantum or remaining time (whichever is smaller)
int execution_time = (processes[proc_index].remaining_time < time_quantum) ?
processes[proc_index].remaining_time : time_quantum;

current_time += execution_time;
processes[proc_index].remaining_time -= execution_time;

printf(" %2d  |   P%d   |        %d       |", current_time - execution_time,
process_id, processes[proc_index].remaining_time);

// Check for newly arrived processes
int new_arrivals = 0;
for (int i = 0; i < n; i++) {
if (processes[i].arrival_time <= current_time && 
processes[i].remaining_time > 0 &&
processes[i].process_id > process_id) {
// Check if not already in queue
int found = 0;
for (int j = queue_front; j < queue_rear; j++) {
if (queue[j].time == i) {
found = 1;
break;
}
}
if (!found) {
queue[queue_rear].process_id = processes[i].process_id;
queue[queue_rear].time = i;
queue_rear++;
new_arrivals++;
}
}
}

if (processes[proc_index].remaining_time == 0) {
// Process completed
processes[proc_index].completion_time = current_time;
processes[proc_index].turnaround_time = 
processes[proc_index].completion_time - processes[proc_index].arrival_time;
processes[proc_index].waiting_time = 
processes[proc_index].turnaround_time - processes[proc_index].burst_time;
printf(" Completed\n");
completed++;
} else {
// Process moved to back of queue
queue[queue_rear].process_id = process_id;
queue[queue_rear].time = proc_index;
queue_rear++;
printf(" Moved to queue\n");
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
int n, time_quantum;
Process processes[MAX_PROCESSES];

printf("========== Round Robin (RR) CPU Scheduling Algorithm ==========\n\n");

// Input number of processes
printf("Enter number of processes (max %d): ", MAX_PROCESSES);
scanf("%d", &n);

if (n <= 0 || n > MAX_PROCESSES) {
printf("Invalid number of processes!\n");
return 1;
}

// Input time quantum
printf("Enter time quantum: ");
scanf("%d", &time_quantum);

if (time_quantum <= 0) {
printf("Invalid time quantum! Using 2 units.\n");
time_quantum = 2;
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

// Calculate Round Robin scheduling
calculateRoundRobin(processes, n, time_quantum);

// Display scheduling table
displaySchedulingTable(processes, n);

// Calculate and display averages
calculateAverages(processes, n);

return 0;
}

/*
* Compilation and Execution:
* gcc -o round_robin 8_round_robin_scheduling.c
* ./round_robin
* 
* Example Input:
* Processes: 3, Time Quantum: 2
* P1: Burst=8, Arrival=0
* P2: Burst=4, Arrival=0
* P3: Burst=2, Arrival=0
* 
* Key Concepts:
* 1. Each process gets a fixed time quantum (time slice)
* 2. Process is moved to back of queue if it doesn't complete
* 3. Executed in FCFS order with time quantum restriction
* 4. If burst time < quantum, process completes in current turn
* 
* Advantages:
* - Fair scheduling (all processes get equal CPU time)
* - Prevents starvation
* - Good for time-sharing systems
* - Better response time for short jobs
* 
* Disadvantages:
* - Higher context switching overhead
* - Average waiting time may be higher than SJF
* - Performance depends on time quantum selection
*/

