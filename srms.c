#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

// Function Prototypes
int loginSystem(void);
void mainMenu(void);
void adminMenu(void);
void userMenu(void);
void staffMenu(void);
void guestMenu(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main() {
    if(loginSystem()) {
        mainMenu();
    } else {
        printf("\nAccess Denied. Exiting…\n");
    }
    return 0;
}

// Login System
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[10];

    printf("===== Login =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if(!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while(fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if(strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// Main Menu
void mainMenu() {
    if(strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if(strcmp(currentRole, "USER") == 0)
        userMenu();
    else if(strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

// Admin Menu
void adminMenu() {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging Out...\n"); return;
            default: printf("Invalid choice...\n");
        }
    } while(choice != 6);
}

// User Menu (can only view students)
void userMenu() {
    int choice;
    do {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging Out...\n"); return;
            default: printf("Invalid choice...\n");
        }
    } while(choice != 3);
}

// Staff Menu (view and search only)
void staffMenu() {
    int choice;
    do {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging Out...\n"); return;
            default: printf("Invalid choice...\n");
        }
    } while(choice != 3);
}

// Guest Menu (view only)
void guestMenu() {
    int choice;
    do {
        printf("\n===== GUEST MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: displayStudents(); break;
            case 2: printf("Logging Out...\n"); return;
            default: printf("Invalid choice...\n");
        }
    } while(choice != 2);
}

// Add Student
void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if(!fp) {
        printf("Error opening student file!\n");
        return;
    }
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf(" %[^\n]%*c", s.name);  // Read string with spaces
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student added successfully!\n");
}

// Display Students
void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if(!fp) {
        printf("No students found.\n");
        return;
    }
    printf("\n%-10s %-30s %-10s\n", "Roll", "Name", "Marks");
    printf("-------------------------------------------------\n");
    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%-10d %-30s %-10.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
}

// Search Student
void searchStudent() {
    int roll, found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if(!fp) {
        printf("No students found.\n");
        return;
    }
    printf("Enter Roll Number to search: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if(s.roll == roll) {
            printf("Student Found: Roll: %d, Name: %s, Marks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    if(!found) printf("Student with Roll %d not found.\n", roll);
    fclose(fp);
}

// Update Student
void updateStudent() {
    int roll, found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if(!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Roll Number to update: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if(s.roll == roll) {
            printf("Enter new Name: ");
            scanf(" %[^\n]%*c", s.name);
            printf("Enter new Marks: ");
            scanf("%f", &s.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if(found) printf("Student updated successfully!\n");
    else printf("Student with Roll %d not found.\n", roll);
}

// Delete Student
void deleteStudent() {
    int roll, found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if(!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Roll Number to delete: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if(s.roll == roll) {
            found = 1;
            continue;  // Skip writing this student
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }
    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if(found) printf("Student deleted successfully!\n");
    else printf("Student with Roll %d not found.\n", roll);
}