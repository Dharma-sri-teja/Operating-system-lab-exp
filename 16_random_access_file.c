#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int emp_id;
    char name[50];
    char designation[50];
    float salary;
    int years_experience;
} Employee;

#define RECORD_SIZE sizeof(Employee)

void create_employee_file(char *filename) {
    FILE *file = fopen(filename, "wb");
    
    if (file == NULL) {
        printf("Error creating file!\n");
        return;
    }
    
    // Create initial empty records
    Employee emp;
    memset(&emp, 0, sizeof(emp));
    
    for (int i = 0; i < 10; i++) {
        fwrite(&emp, RECORD_SIZE, 1, file);
    }
    
    fclose(file);
    printf("Employee file created with 10 empty records\n");
}

void insert_employee(char *filename, int record_num, Employee *emp) {
    FILE *file = fopen(filename, "r+b");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    // Seek to the desired record position
    fseek(file, (record_num - 1) * RECORD_SIZE, SEEK_SET);
    
    if (fwrite(emp, RECORD_SIZE, 1, file) != 1) {
        printf("Error writing to file!\n");
    } else {
        printf("Employee record #%d inserted successfully\n", record_num);
    }
    
    fclose(file);
}

void display_employee(char *filename, int record_num) {
    FILE *file = fopen(filename, "rb");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    Employee emp;
    
    // Seek to the desired record position
    fseek(file, (record_num - 1) * RECORD_SIZE, SEEK_SET);
    
    if (fread(&emp, RECORD_SIZE, 1, file) != 1) {
        printf("Error reading from file!\n");
    } else {
        if (emp.emp_id == 0) {
            printf("Record #%d is empty\n", record_num);
        } else {
            printf("\n--- Employee Record #%d ---\n", record_num);
            printf("ID: %d\n", emp.emp_id);
            printf("Name: %s\n", emp.name);
            printf("Designation: %s\n", emp.designation);
            printf("Salary: %.2f\n", emp.salary);
            printf("Years of Experience: %d\n\n", emp.years_experience);
        }
    }
    
    fclose(file);
}

void update_employee(char *filename, int record_num, Employee *emp) {
    FILE *file = fopen(filename, "r+b");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    fseek(file, (record_num - 1) * RECORD_SIZE, SEEK_SET);
    
    if (fwrite(emp, RECORD_SIZE, 1, file) != 1) {
        printf("Error updating record!\n");
    } else {
        printf("Employee record #%d updated successfully\n", record_num);
    }
    
    fclose(file);
}

void delete_employee(char *filename, int record_num) {
    FILE *file = fopen(filename, "r+b");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    Employee emp;
    memset(&emp, 0, sizeof(emp));
    
    fseek(file, (record_num - 1) * RECORD_SIZE, SEEK_SET);
    
    if (fwrite(&emp, RECORD_SIZE, 1, file) != 1) {
        printf("Error deleting record!\n");
    } else {
        printf("Employee record #%d deleted successfully\n", record_num);
    }
    
    fclose(file);
}

void display_all_employees(char *filename) {
    FILE *file = fopen(filename, "rb");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    Employee emp;
    int count = 0;
    
    printf("\n============ All Employee Records ============\n");
    printf("Rec# | ID    | Name                | Designation      | Salary  | Experience\n");
    printf("===================================================================\n");
    
    for (int i = 1; i <= 10; i++) {
        fseek(file, (i - 1) * RECORD_SIZE, SEEK_SET);
        
        if (fread(&emp, RECORD_SIZE, 1, file) == 1 && emp.emp_id != 0) {
            printf("%4d | %5d | %-18s | %-16s | %7.2f | %d\n", 
                   i, emp.emp_id, emp.name, emp.designation, emp.salary, emp.years_experience);
            count++;
        }
    }
    
    printf("===================================================================\n");
    printf("Total active records: %d\n\n", count);
    
    fclose(file);
}

int main() {
    char filename[] = "employees.dat";
    
    printf("Random Access File - Employee Management System\n");
    printf("================================================\n\n");
    
    // Create the employee file
    create_employee_file(filename);
    
    // Create some employee records
    Employee emp1 = {101, "Ravi Kumar", "Software Engineer", 50000.0, 5};
    Employee emp2 = {102, "Priya Singh", "Manager", 75000.0, 8};
    Employee emp3 = {103, "Amit Patel", "Analyst", 45000.0, 3};
    Employee emp4 = {104, "Neha Sharma", "Developer", 55000.0, 6};
    
    // Insert employees at specific record numbers
    insert_employee(filename, 1, &emp1);
    insert_employee(filename, 2, &emp2);
    insert_employee(filename, 3, &emp3);
    insert_employee(filename, 4, &emp4);
    
    // Display specific employee records
    printf("\nDisplaying employee records:\n");
    display_employee(filename, 1);
    display_employee(filename, 3);
    
    // Display all employees
    display_all_employees(filename);
    
    // Update an employee
    emp1.salary = 60000.0;
    emp1.years_experience = 6;
    update_employee(filename, 1, &emp1);
    
    // Display updated record
    display_employee(filename, 1);
    
    // Delete an employee
    delete_employee(filename, 2);
    printf("\n");
    
    // Display all employees after deletion
    display_all_employees(filename);
    
    printf("File processing completed successfully!\n");
    
    return 0;
}
