#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float salary;
} Employee;

void demonstrate_malloc() {
    printf("\n=== malloc() - Allocates uninitialized memory ===\n");
    
    Employee *emp = (Employee *)malloc(sizeof(Employee));
    
    if (emp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    emp->id = 1;
    strcpy(emp->name, "John Doe");
    emp->salary = 50000.0;
    
    printf("Employee ID: %d\n", emp->id);
    printf("Employee Name: %s\n", emp->name);
    printf("Employee Salary: %.2f\n", emp->salary);
    
    free(emp);
    printf("Memory freed\n");
}

void demonstrate_calloc() {
    printf("\n=== calloc() - Allocates and initializes memory to zero ===\n");
    
    int n = 5;
    Employee *employees = (Employee *)calloc(n, sizeof(Employee));
    
    if (employees == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    printf("Allocated memory for %d employees (initialized to zero)\n", n);
    
    for (int i = 0; i < n; i++) {
        printf("Employee %d - ID: %d, Name: %s, Salary: %.2f\n", 
               i, employees[i].id, employees[i].name, employees[i].salary);
    }
    
    free(employees);
    printf("Memory freed\n");
}

void demonstrate_realloc() {
    printf("\n=== realloc() - Resizes previously allocated memory ===\n");
    
    int *arr = (int *)malloc(5 * sizeof(int));
    
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    // Fill initial array
    for (int i = 0; i < 5; i++) {
        arr[i] = i * 10;
    }
    
    printf("Initial array (size 5): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Resize array to 10 elements
    arr = (int *)realloc(arr, 10 * sizeof(int));
    
    if (arr == NULL) {
        printf("Memory reallocation failed!\n");
        return;
    }
    
    // Initialize new elements
    for (int i = 5; i < 10; i++) {
        arr[i] = i * 10;
    }
    
    printf("Resized array (size 10): ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
    printf("Memory freed\n");
}

void demonstrate_static_allocation() {
    printf("\n=== Static/Stack Allocation ===\n");
    
    Employee emp;
    emp.id = 1;
    strcpy(emp.name, "Jane Smith");
    emp.salary = 60000.0;
    
    printf("Employee ID: %d\n", emp.id);
    printf("Employee Name: %s\n", emp.name);
    printf("Employee Salary: %.2f\n", emp.salary);
    printf("(No explicit free needed - automatic cleanup when scope ends)\n");
}

int main() {
    printf("Memory Allocation Strategies in C\n");
    printf("==================================\n");
    
    demonstrate_malloc();
    demonstrate_calloc();
    demonstrate_realloc();
    demonstrate_static_allocation();
    
    printf("\n=== Summary ===\n");
    printf("malloc(): Allocates memory, contents are uninitialized\n");
    printf("calloc(): Allocates memory, initializes to zero, slower\n");
    printf("realloc(): Resizes previously allocated memory\n");
    printf("free(): Deallocates dynamic memory\n");
    printf("Static: Stack allocation, automatic deallocation\n");
    
    return 0;
}
