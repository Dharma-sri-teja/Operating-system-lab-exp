/*
* Program: First Come First Served (FCFS) CPU Scheduling
* Description: Implements FCFS scheduling algorithm where:
* - All processes are activated at time 0
* - No process waits on I/O devices
* - Processes are executed in the order they arrive
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

typedef struct {
int process_id;
int burst_time;
int arrival_time;
int completion_time;
int turnaround_time;
int waiting_time;
} Process;

void calculateFCFS(Process processes[], int n) {
int current_time = 0;

printf("\n===== FCFS Scheduling =====\n\n");
printf("Process | Burst Time | Completion Time | Turnaround Time | Waiting Time\n");
printf("--------|------------|-----------------|-----------------|--------------\n");

for (int i = 0; i < n; i++) {
// Each process starts after the previous one completes
current_time += processes[i].burst_time;
processes[i].completion_time = current_time;

// Turnaround time = Completion time - Arrival time
processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;

// Waiting time = Turnaround time - Burst time
processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

printf("   P%d   |     %d      |        %d        |       %d        |      %d\n",
processes[i].process_id,
processes[i].burst_time,
processes[i].completion_time,
processes[i].turnaround_time,
processes[i].waiting_time);
}
}

void displayGanttChart(Process processes[], int n) {
printf("\n===== Gantt Chart =====\n");
printf("Timeline: ");

int time = 0;
for (int i = 0; i < n; i++) {
printf("%d ", time);
time = processes[i].completion_time;
}
printf("%d\n", time);

printf("Process:  ");
for (int i = 0; i < n; i++) {
printf("|  P%d  ", processes[i].process_id);
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

printf("========== FCFS CPU Scheduling Algorithm ==========\n");
printf("Note: All processes arrive at time 0\n\n");

// Input number of processes
printf("Enter number of processes (max %d): ", MAX_PROCESSES);
scanf("%d", &n);

if (n <= 0 || n > MAX_PROCESSES) {
printf("Invalid number of processes!\n");
return 1;
}

// Input process details
printf("\nEnter process burst time (arrival time = 0 for all):\n");
for (int i = 0; i < n; i++) {
processes[i].process_id = i + 1;
processes[i].arrival_time = 0;  // All processes arrive at time 0

printf("Process P%d burst time: ", i + 1);
scanf("%d", &processes[i].burst_time);

if (processes[i].burst_time <= 0) {
printf("Invalid burst time! Using 1 unit.\n");
processes[i].burst_time = 1;
}
}

// Calculate FCFS scheduling
calculateFCFS(processes, n);

// Display Gantt chart
displayGanttChart(processes, n);

// Calculate and display averages
calculateAverages(processes, n);

return 0;
}

/*
* Compilation and Execution:
* gcc -o fcfs_scheduling 3_fcfs_scheduling.c
* ./fcfs_scheduling
* 
* Example Input:
* Number of processes: 3
* Process burst times: 8, 4, 2
* 
* Key Concepts:
* 1. All processes arrive at time 0
* 2. Processes execute in order of arrival (FIFO)
* 3. Completion Time = Previous Completion Time + Current Burst Time
* 4. Turnaround Time = Completion Time - Arrival Time
* 5. Waiting Time = Turnaround Time - Burst Time
* 
* Advantages:
* - Simple to implement and understand
* - No starvation
* 
* Disadvantages:
* - Long waiting times (convoy effect)
* - Not optimal for average waiting time
*/

